#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#define MAX_FRAMES 126


struct packet
{
    uint8_t length;
    uint8_t payload[MAX_FRAMES];
    uint8_t padding;
};

typedef struct packet packet;

int create_packet(uint8_t* data_in, uint8_t length, packet* out);

#endif