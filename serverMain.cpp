#include <bits/stdc++.h>
using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "socket.h"
#include "message.h"
#include "network.h"

unsigned int seq = 0;

int main() {
  char* mode = (char*) "enp39s0";
  int soc = ConexaoRawSocket(mode);

  while(1){
    vector<Message*> messages;
    for(int i = 0; i < 1; i++){
      Mask* ma = listenType(soc, ANY);
      messages.push_back(maskToMessage(ma));
      delete ma;
    }

    // processing message
    Message* me = messages[0];
    if(me->seq < seq){
      Mask ma;
      ma.type = OK;
      ma.marker = MARKER;
      write(soc, &ma, sizeof(Mask));
      delete me;
      continue;
    }

    for(auto m : messages){
      cout << m->seq << ": ";
      for(int i = 0; i < m->size; i++){
        cout << m->buff[i];
      }
      cout << '\n';
      delete m;
    }
    // end of process

    int a = rand() % 20;
    if(a == 2){continue;}

    Mask ma;
    ma.type = OK;
    ma.marker = MARKER;
    write(soc, &ma, sizeof(Mask));

    if(seq == 15) cout << '\n';
    seq = (seq + 1) % 16;
  }

  
  return 0;
}