#include "communication.h"
#include "sys.h"

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
    // Serial.print("Received COM = ");
    // Serial.println((uint8_t)(&comm->serial));
    if (size_t avail_bytes = comm->serial->available(); avail_bytes != 0) 
    {
        comm->serial->readBytes((char*)comm->raw_buf, avail_bytes);
        memcpy(&id, comm->raw_buf, sizeof(id));
        memcpy(&size, comm->raw_buf + sizeof(id), sizeof(size));
        memcpy(&crc, comm->raw_buf + sizeof(id) + sizeof(size) + size, sizeof(crc));
        calc_crc = crc16(0xffff, comm->raw_buf, size + sizeof(id) + sizeof(size) );
        
        if (comm->recv_buf[id] != NULL && comm->recv_len[id] == size && calc_crc == crc)
        {
            // Serial.print("RECV ID =");
            // Serial.println(id);
            memcpy(comm->recv_buf[id], comm->raw_buf + sizeof(id) + sizeof(size), size);
            comm->recv_time[id] = get_time_ms();
            // call callback
            if (comm->recv_callback[id] != NULL)
                comm->recv_callback[id]();
        }
        else 
        {
            // Serial.println("ERROR!");
            // comm->recv_flag[id] = 0;
        }
    }
}


int communication_register_recv(communication_t comm, uint8_t pkt_id, void* data_ptr, size_t len, recv_callback_t callback)
{
    if (comm->recv_buf[pkt_id] != NULL)
        return -1;

    comm->recv_buf[pkt_id] = data_ptr;
    comm->recv_len[pkt_id] = len;
    comm->recv_callback[pkt_id] = callback;
    
    return 0;
}

const uint16_t COMM_EOF = 0xffd9;

int communication_send(communication_t comm, uint8_t pkt_id, const void* data_ptr, size_t len)
{
    if (data_ptr == NULL)
        return -1;
 
    uint8_t id = pkt_id;
    uint16_t size = len;
    uint16_t crc;

    size_t ofst = 0;
    memcpy(comm->raw_buf + ofst, &id, sizeof(id));
    ofst += sizeof(id);
    memcpy(comm->raw_buf + ofst, &size, sizeof(size));
    ofst += sizeof(size);
    memcpy(comm->raw_buf + ofst, data_ptr, size);
    crc = crc16(0xffff, comm->raw_buf, size + sizeof(id) + sizeof(size));
    
    ofst += size;
    memcpy(comm->raw_buf + ofst, &crc, sizeof(crc));
    
    // set EOF
    ofst += sizeof(crc);
    memcpy(comm->raw_buf + ofst, &COMM_EOF, sizeof(COMM_EOF));
    
    ofst += sizeof(COMM_EOF);

    if (comm->serial->write((char*)comm->raw_buf, ofst) == ofst)
        return 0;
    
    return -1;
}

uint32_t communication_get_recv_time_ms(communication_t comm, uint8_t pkt_id)
{
    return comm->recv_time[pkt_id];
}