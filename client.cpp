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

  char *s = (char*)"batata";

  while(1){
    Mask b;
    b.marker = MARKER;
    b.size = 6;
    b.seq = clientSeq;
    b.type = 0b110000;
    b.parity = 0b00000000;
    for(int i = 0; i < 6; i++)
      b.buff[i] = (long int) s[i];


    memcpy(&buffer, &b, sizeof(Message));
    write(soc, buffer, BUFFER_SIZE);

    
    if(clientSeq == 15) sleep(2);
    clientSeq = (clientSeq + 1) % 16;
  }


  return 0;
}