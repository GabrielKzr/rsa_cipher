#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <stdio.h>

#define MAX_FRAMES  128
#define MAX_PAYLOAD 126


typedef union {
    struct {
        uint8_t length;
        uint8_t payload[MAX_PAYLOAD];
        uint8_t padding;
    } s;
    uint8_t bytes[sizeof(uint8_t) + MAX_PAYLOAD + sizeof(uint8_t)];
} packet;

int create_packet(uint8_t* data_in, uint8_t length, packet* out);
int unpack_packet(packet* in, uint8_t* data_out);
void print_packet(packet* p);

#endif