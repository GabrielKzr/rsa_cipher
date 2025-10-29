#include "packet.h"

int create_packet(uint8_t* data_in, uint8_t length, packet* out) {
    
    if(!out) return -1;
    if(!data_in) return -1;
    if(length > MAX_FRAMES) return -1;

    out->length = length;

    for(int i = 0; i < length; i++) {
        out->payload[i] = data_in[i];
    }

    out->padding = 0x00;

    return 0;
}