#include "packet.h"

int create_packet(uint8_t* data_in, uint8_t length, packet* out) {
    
    if(!out) return -1;
    if(!data_in) return -1;
    if(length > MAX_PAYLOAD) return -1;

    out->s.length = length;

    int i;
    for(i = 0; i < length; i++) {
        out->s.payload[i] = data_in[i];
    }

    for(int j = i; j < MAX_PAYLOAD; j++) {
        out->s.payload[j] = 0x00;
    }

    out->s.padding = 0x00;

    return 0;
}

int unpack_packet(packet* in, uint8_t* data_out) {

    if(!in) return -1;
    if(!data_out) return -1;
    
    for(int i = 0; i < in->s.length; i++) {
        data_out[i] = in->s.payload[i];
    }

    return 0;
}

void print_packet(packet* p) {

    if(!p) return;

    for(int i = 0; i < MAX_FRAMES; i++) {
        if(!i) printf("%d. %d\n", i, p->bytes[i]);
        else if(i == MAX_FRAMES-1) printf("%d. 0x00\n", i);
        else printf("%d. %c\n", i, (char)p->bytes[i]);
    }
}