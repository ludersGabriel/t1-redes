#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "socket.h"

// 11111111 0000 1111 0000
#define MARKER 0b00000011

typedef struct __attribute__((packed)) test {
    unsigned int marker : 8;
    unsigned int size : 6;
    unsigned int seq : 4;
    unsigned int type : 6;
} message;

unsigned int seq = 0;

int main() {
  int soc = ConexaoRawSocket("lo");
  unsigned char buffer[30];

  while(1){
    recvfrom(soc, &buffer, 30, 0, NULL, NULL);

    message header;
    memcpy(&header, &buffer, sizeof(message));
    
    if(header.marker != MARKER) continue;
    if(header.seq != seq) continue;

    seq = (seq + 1) % 16;

    printf("%d\n", header.marker);
    printf("%d\n", header.seq);
    printf("%d\n", header.size);
    printf("%d\n\n", header.type);
  }

  
  return 0;
}