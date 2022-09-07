#include <bits/stdc++.h>
using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "socket.h"
#include "message.h"

#define BUFFER_SIZE 600

int main(){
  int soc = ConexaoRawSocket((char*)"enp3s0");

  unsigned char buffer[BUFFER_SIZE];
  unsigned int clientSeq = 0;

  while(1){
    Mask b;
    b.marker = MARKER;
    b.size = 0b000011;
    b.seq = clientSeq;
    b.type = 0b110000;
    b.parity = 0b00000000;

    printf("%ld\n\n", sizeof(b));

    memcpy(&buffer, &b, sizeof(Message));
    write(soc, buffer, BUFFER_SIZE);
    
    clientSeq = (clientSeq + 1) % 16;
  }


  return 0;
}