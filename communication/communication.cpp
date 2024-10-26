#include "communication.h"

void communication_setup(communication_t comm, HardwareSerial* serial, size_t max_bytes)
{
    comm->serial = serial;
    comm->max_bytes = max_bytes;
    comm->raw_buf = (void*)malloc(max_bytes);
}

void communication_loop(communication_t comm)
{
    uint8_t id;
    uint16_t size;
    uint16_t crc, calc_crc;

    // Receive
    if (size_t avail_bytes = comm->serial->available(); avail_bytes != 0) 
    {
        comm->serial->readBytes((char*)comm->raw_buf, avail_bytes);
        memcpy(&id, comm->raw_buf, sizeof(id));
        memcpy(&size, comm->raw_buf + sizeof(id), sizeof(size));
        memcpy(&crc, comm->raw_buf + sizeof(id) + sizeof(size) + size, sizeof(crc));
        calc_crc = crc16(0xffff, comm->raw_buf, size + sizeof(id) + sizeof(size) );
        
        // Serial.println("Avail!");
        // Serial.println(id);
        // Serial.println(size);
        // Serial.println(crc);
        // Serial.println(calc_crc);

        if (comm->recv_buf[id] != NULL && comm->recv_len[id] == size && calc_crc == crc)
        {
            // Serial.println("MemCpy!");
            memcpy(comm->recv_buf[id], comm->raw_buf + sizeof(id) + sizeof(size), size);
            comm->recv_flag[id] = 1;
        }
        else 
        {
            comm->recv_flag[id] = 0;
        }
    }

    // Send 
    for (uint8_t id = 0; id < COMM_SEND_MAX_NUM; id++)
    {
        if (comm->send_buf[id] != NULL && comm->send_flag[id])
        {
            size = comm->send_len[id];

            memcpy(comm->raw_buf, &id, sizeof(id));
            memcpy(comm->raw_buf + sizeof(id), &size, sizeof(size));
            memcpy(comm->raw_buf + sizeof(id) + sizeof(size), comm->send_buf[id], size);

            crc = crc16(0xffff, comm->raw_buf, size + sizeof(id) + sizeof(size));
            memcpy(comm->raw_buf + sizeof(id) + sizeof(size) + size, &crc, sizeof(crc));

            size_t whole_size = sizeof(id)+sizeof(size)+size+sizeof(crc);
            if (comm->serial->write((char*)comm->raw_buf, whole_size) == whole_size)
                comm->send_flag[id] = 0;
        }
    }
}

int communication_register_send_buf(communication_t comm, uint8_t pkt_id, size_t len)
{
    if (comm->send_buf[pkt_id] != NULL)
        return -1;

    comm->send_buf[pkt_id] = (void*)malloc(len);
    comm->send_flag[pkt_id] = 0;
    comm->send_len[pkt_id] = len;

    return 0;
}


int communication_register_recv_buf(communication_t comm, uint8_t pkt_id, size_t len)
{
    if (comm->recv_buf[pkt_id] != NULL)
        return -1;

    comm->recv_buf[pkt_id] = (void*)malloc(len);
    comm->recv_flag[pkt_id] = 0;
    comm->recv_len[pkt_id] = len;
    
    return 0;
}

int communication_send(communication_t comm, uint8_t pkt_id, const void* data_ptr)
{
    if (comm->send_buf[pkt_id] == NULL)
        return -1;
    memcpy(comm->send_buf[pkt_id], data_ptr, comm->send_len[pkt_id]);
    comm->send_flag[pkt_id] = 1;
    return 0;
}

int communication_recv(communication_t comm, uint8_t pkt_id, void* data_ptr)
{
    if (comm->recv_buf[pkt_id] == NULL)
        return -1;
    if (comm->recv_flag[pkt_id] == 0)
        return -1;
    
    // Serial.println("MemCpyToDst!");
    // Serial.println(((uint8_t*)comm->recv_buf[pkt_id])[0]);
    memcpy(data_ptr, comm->recv_buf[pkt_id], comm->recv_len[pkt_id]);
    return 0;
}