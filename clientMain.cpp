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
#include "network.h"

#define TIMEOUT 1

bool timedOut = false;

void handler(int signum){
  timedOut = true;
}

int main(){
  int soc = ConexaoRawSocket((char*)"enp3s0");
  timedOut = false;

  struct sigaction action;
  action.sa_handler = handler;
  sigemptyset (&action.sa_mask) ;
  action.sa_flags = 0 ;
  sigaction (SIGALRM, &action, 0);

  unsigned int clientSeq = 0;

  char *s = (char*)"batata";

  while(1){
    Mask *b = new Mask();
    b->marker = MARKER;
    b->size = 6;
    b->seq = clientSeq;
    b->type = 0b110000;
    b->parity = 0b00000000;
    for(int i = 0; i < 6; i++)
      b->buff[i] = (long int) s[i];

    write(soc, b, sizeof(Mask));
    
    Mask* ma = listenWithTimeout(timedOut, soc, b, OK);

    delete ma;
    delete b;

    clientSeq = (clientSeq + 1) % 16;
  }


  return 0;
}