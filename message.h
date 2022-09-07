#ifndef MESSAGE_HEADER
#define MESSAGE_HEADER

typedef struct __attribute__((packed)) Message_s {
    unsigned int marker : 8;
    unsigned int size : 6;
    unsigned int seq : 4;
    unsigned int type : 6;
    unsigned char buff[1 << 6];
    unsigned int parity : 8;
} Message;

typedef struct __attribute__((packed)) Mask_s {
    unsigned int marker : 8;
    unsigned int size : 6;
    unsigned int seq : 4;
    unsigned int type : 6;
    unsigned long int buff[1 << 6];
    unsigned int parity : 8;
} Mask;

#define MARKER 0b01111110


#endif