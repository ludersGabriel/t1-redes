#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include "socket.h"

// 11111111 0000 1111 0000

typedef struct __attribute__((packed)) test {
    unsigned int marker : 8;
    unsigned int size : 6;
    unsigned int seq : 4;
    unsigned int type : 6;
} message;

int main() {
  printf("%ld\n", sizeof(message));


  return 0;
  int soc = ConexaoRawSocket("lo");
  unsigned char mess[30];

  message *st = (message *) 0xAF0F000A;

  printf("%d\n", st->marker);

  return 0;

  while(1){
    recvfrom(soc, &mess, 30, 0, NULL, NULL);
    printf("%s\n", mess);
  }

  
  return 0;
}