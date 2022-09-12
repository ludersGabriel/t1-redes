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
    Mask* ma = listenType(soc, ANY);

    if(ma->seq < clientSeq) continue;

    Message* recMe = maskToMessage(ma);

    switch (recMe->type){
      case LS: {
        cout << "[+] received LS: " << recMe->seq << endl;
        string ls = "ls ";
        for(int i = 0; i < recMe->size; i++)
          ls += recMe->buff[i];
        sendLS(ls);
        break;
      }
      default:
        break;
    }
    
    delete ma;
    delete recMe;
    clientSeq = (clientSeq + 1) % 16;
  }

  
  return 0;
}