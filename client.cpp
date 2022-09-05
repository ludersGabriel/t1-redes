#include <bits/stdc++.h>
using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "socket.h"

#define MARKER 0b01111110

typedef struct __attribute__((packed)) test {
    unsigned int marker : 8;
    unsigned int size : 6;
    unsigned int seq : 4;
    unsigned int type : 6;
    unsigned char buff[2 << 5]
} message;

int main(){
  int soc = ConexaoRawSocket((char*)"lo");

  unsigned int initSeq = 0b0000;

  message b;
  b.marker = MARKER;
  b.size = 0b000011;
  b.seq = initSeq;
  b.type = 0b110000;

  printf("%d\n\n", b.marker == MARKER);

  unsigned char message[30];
  while(1){
    memcpy(&message, &b, sizeof(message));
    write(soc, message, 30);
    
    if(b.seq == 0b1111) b.seq = initSeq;
    else b.seq++;
  }


  return 0;
}