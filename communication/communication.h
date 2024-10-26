#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include <stdint.h>
#include <Arduino.h>

#define COMM_SEND_MAX_NUM (16)
#define COMM_RECV_MAX_NUM (16)
typedef struct communication* communication_t;

struct communication
{
    HardwareSerial* serial;

    void* send_buf[COMM_SEND_MAX_NUM];
    uint8_t send_flag[COMM_SEND_MAX_NUM];
    uint16_t send_len[COMM_SEND_MAX_NUM];

    void* recv_buf[COMM_RECV_MAX_NUM];
    uint8_t recv_flag[COMM_RECV_MAX_NUM];
    uint16_t recv_len[COMM_RECV_MAX_NUM];


    // raw buffer
    void* raw_buf;

    size_t max_bytes;

    unsigned long last_update_time;
    unsigned long period_us;

    int state;
};

void communication_setup(communication_t comm, HardwareSerial* serial, size_t max_bytes);
void communication_loop(communication_t comm);

int communication_register_send_buf(communication_t comm, uint8_t pkt_id, size_t len);
int communication_register_recv_buf(communication_t comm, uint8_t pkt_id, size_t len);

int communication_send(communication_t comm, uint8_t pkt_id, const void* data_ptr);
int communication_recv(communication_t comm, uint8_t pkt_id, void* data_ptr);

uint16_t crc16(uint16_t crc, const void *buf, size_t size);

#endif 