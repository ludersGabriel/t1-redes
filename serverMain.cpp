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

Message* maskToMessage(Mask* ma){
  Message* me = new Message();
  me->marker = ma->marker;
  me->parity = ma->parity;
  me->seq = ma->seq;
  me->size = ma->size;
  me->type = ma->type;
  for(int i = 0; i < 1 << 6; i++){
    me->buff[i] = (unsigned char) ma->buff[i];
  }

  return me;
}


int main() {
  char* mode = (char*) "enp39s0";
  int soc = ConexaoRawSocket(mode);

  ::clientSeq = 0;
  ::serverSeq = 0;
  ::timedOut = 0;

  while(1){
    Message* recMe = maskToMessage(listenType(soc, ANY));

    switch (recMe->type){
      case LS: {
        cout << "[+] enviando LS\n";
        system("ls > temp.txt");
        ifstream temp("./temp.txt");

        while(temp){
          Mask *resp = new Mask(SHOW, ::serverSeq);
          
          char c;
          int i = 0;
          resp->size = 0;
          for(i = 0; temp && i < BUFFER_SIZE; i++){
            c = temp.get();
            resp->buff[i] = c;
          }

          resp->size = i - 1;

          write(soc, resp, sizeof(Mask));
          serverSeq = (serverSeq + 1) % 16;      

          Mask* ack = listenWithTimeout(
            timedOut, 
            soc, 
            resp,
            ANY
          );

          delete ack;
          delete resp;
        }

        temp.close();
        system("rm ./temp.txt");
        break;
      }
      default:
        break;
    }

    clientSeq = (clientSeq + 1) % 16;
  }

  
  return 0;
}