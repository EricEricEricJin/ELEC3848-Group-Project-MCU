#include "communication.h"
#include "sys.h"

const uint16_t COMM_SOF = 0xffd8;
const uint16_t COMM_EOF = 0xffd9;

void communication_setup(communication_t comm, HardwareSerial* serial, size_t recv_max_bytes, size_t send_max_bytes)
{
    comm->serial = serial;

    comm->raw_buf = (void*)malloc(recv_max_bytes);
    comm->raw_buf_idx = 0;
    comm->raw_buf_max_bytes =recv_max_bytes;

    comm->send_buf = (void*)malloc(send_max_bytes);
    comm->send_buf_max_bytes = send_max_bytes;
}

void unpack(communication_t comm, const void* ptr, size_t len)
{
    Serial.println("Unpack");
    uint8_t id;
    uint16_t size;
    uint16_t crc, calc_crc;

    if (len < sizeof(id) + sizeof(size) + sizeof(crc))
        return;

    
    // Receive
    // Serial.print("Received COM = ");
    // Serial.println((uint8_t)(&comm->serial));
    memcpy(&id, ptr, sizeof(id));
    memcpy(&size, ptr + sizeof(id), sizeof(size));

    if (sizeof(id) + sizeof(size) + size + sizeof(crc) != len)
    {
        // Serial.println("ERR LEN");
        return;
    }

    memcpy(&crc, ptr + sizeof(id) + sizeof(size) + size, sizeof(crc));
    calc_crc = crc16(0xffff, ptr, size + sizeof(id) + sizeof(size));
    
    // Serial.print("RECV ID =");
    // Serial.println(id);
    // Serial.println(size);

    if (id < COMM_RECV_MAX_NUM && comm->recv_buf[id] != NULL && comm->recv_len[id] == size && calc_crc == crc)
    {

        memcpy(comm->recv_buf[id], ptr + sizeof(id) + sizeof(size), size);
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

bool communication_loop(communication_t comm)
{
    // Serial.println("CommLoop");
    if (int avail_bytes = comm->serial->available(); avail_bytes != 0) 
    {
        size_t apped_len = avail_bytes + comm->raw_buf_idx;
        if (apped_len > comm->raw_buf_max_bytes)
        {
            // overflow
            return apped_len - comm->raw_buf_max_bytes;
        }

        // append bytes to raw buffer
        char* raw_buf = (char*)comm->raw_buf;
        comm->serial->readBytes(&(raw_buf[comm->raw_buf_idx]), avail_bytes);
        comm->raw_buf_idx += avail_bytes;

        // find eof and sof
        int sof_idx = -1;
        int eof_idx = -1;
        uint16_t nxt_two_bytes;

        for (int i = 0; i < comm->raw_buf_idx - 1; i++)
        {
            // nxt_two_bytes = raw_buf[i] | (((uint16_t)raw_buf[i+1]) << 8);
            memcpy(&nxt_two_bytes, &(raw_buf[i]), 2);
            if (nxt_two_bytes == COMM_SOF)
            {
                sof_idx = i;
            }
            else if (nxt_two_bytes == COMM_EOF)
            {
                eof_idx = i;
                if (sof_idx >= 0)
                {
                    // Serial.print("SOF = ");
                    // Serial.print(sof_idx);
                    // Serial.print(", EOF = ");
                    // Serial.println(eof_idx);
                    
                    // for (int j = 0; j < (eof_idx - sof_idx); j++)
                    // {
                    //     Serial.print((uint8_t)raw_buf[sof_idx + j]);
                    //     Serial.print(" ");
                    // }
                    // Serial.println();
                    
                    unpack(comm, (const void*)&(raw_buf[sof_idx + 2]), eof_idx - sof_idx - 2);
                    sof_idx = -1;
                }
            }
        }
        
        if (eof_idx >= 0)
        {
            if (eof_idx != comm->raw_buf_idx - 2)
                memmove(raw_buf, &(raw_buf[eof_idx + 2]), comm->raw_buf_idx - (eof_idx + 2));
            comm->raw_buf_idx -= (eof_idx + 2);
        }
    }

}

/*
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
*/


int communication_register_recv(communication_t comm, uint8_t pkt_id, void* data_ptr, size_t len, recv_callback_t callback)
{
    if (comm->recv_buf[pkt_id] != NULL)
        return -1;

    comm->recv_buf[pkt_id] = data_ptr;
    comm->recv_len[pkt_id] = len;
    comm->recv_callback[pkt_id] = callback;
    
    return 0;
}

int communication_send(communication_t comm, uint8_t pkt_id, const void* data_ptr, size_t len)
{
    if (data_ptr == NULL)
        return -1;
 
    uint8_t id = pkt_id;
    uint16_t size = len;
    uint16_t crc;

    size_t ofst = 0;

    memcpy(comm->send_buf + ofst, &COMM_SOF, sizeof(COMM_SOF));
    ofst += sizeof(COMM_SOF);

    memcpy(comm->send_buf + ofst, &id, sizeof(id));
    ofst += sizeof(id);
    memcpy(comm->send_buf + ofst, &size, sizeof(size));
    ofst += sizeof(size);

    memcpy(comm->send_buf + ofst, data_ptr, size);
    ofst += size;

    crc = crc16(0xffff, comm->send_buf + sizeof(COMM_SOF), size + sizeof(id) + sizeof(size));    
    
    memcpy(comm->send_buf + ofst, &crc, sizeof(crc));
    ofst += sizeof(crc);
    
    // set EOF
    memcpy(comm->send_buf + ofst, &COMM_EOF, sizeof(COMM_EOF));
    ofst += sizeof(COMM_EOF);

    if (ofst > comm->send_buf_max_bytes)
        return -1;
    
    if (comm->serial->write((char*)comm->send_buf, ofst) == ofst)
        return 0;
    
    return -1;
}

uint32_t communication_get_recv_time_ms(communication_t comm, uint8_t pkt_id)
{
    return comm->recv_time[pkt_id];
}