#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include <stdint.h>
#include <Arduino.h>

#define COMM_SEND_MAX_NUM (16)
#define COMM_RECV_MAX_NUM (16)

typedef struct communication* communication_t;
typedef void (*recv_callback_t)();

struct communication
{
    HardwareSerial* serial;
    size_t max_bytes;

    void* recv_buf[COMM_RECV_MAX_NUM];
    uint16_t recv_len[COMM_RECV_MAX_NUM];

    uint32_t recv_time[COMM_RECV_MAX_NUM];
    recv_callback_t recv_callback[COMM_RECV_MAX_NUM]; 

    // raw buffer
    void* raw_buf;
};

void communication_setup(communication_t comm, HardwareSerial* serial, size_t max_bytes);
void communication_loop(communication_t comm);


int communication_send(communication_t comm, uint8_t pkt_id, const void* data_ptr, size_t len);
int communication_register_recv(communication_t comm, uint8_t pkt_id, void* data_ptr, size_t len, recv_callback_t callback);


uint16_t crc16(uint16_t crc, const void *buf, size_t size);

#endif 