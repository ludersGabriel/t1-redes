#include <bits/stdc++.h>
using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "server.h"
#include "socket.h"
#include "message.h"
#include "network.h"

void handler(int signum){
  ::timedOut = true;
}

int main() {
  char* mode = (char*) "enp39s0";
  ::soc = ConexaoRawSocket(mode);

  struct sigaction action;
  action.sa_handler = handler;
  sigemptyset (&action.sa_mask) ;
  action.sa_flags = 0 ;
  sigaction (SIGALRM, &action, 0);

  ::clientSeq = 0;
  ::serverSeq = 0;


  while(1){
    Message* recMe = maskToMessage(listenType(soc, ANY));

    switch (recMe->type){
      case LS: {
        
        FILE* stream = popen("ls", "r");
        sendStream(stream, SHOW);
        sendEnd();
        
        break;
      }
      default:
        break;
    }

    clientSeq = (clientSeq + 1) % 16;
  }

  
  return 0;
}