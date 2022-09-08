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

enum messageTypes {
    OK    = 0b000001,
    NACK  = 0b000010,
    CD    = 0b000110,
    LS    = 0b000111,
    ERROR = 0b010001,
    SHOW  = 0b111111,
    ACK   = 0b000011,
    END   = 0b101110,
    MKDIR = 0b001000,
    GET   = 0b001001,
    DESC  = 0b011000,
    DATA  = 0b100000,
    PUT   = 0b001010,
    ANY   = 0b110101
};

enum errorTypes {
    NO_DIR   = 'A',
    NO_PERM  = 'B',
    DUP_DIR  = 'C',
    NO_FILE  = 'D',
    NO_SPACE = 'E'
};

#endif