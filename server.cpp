#include <bits/stdc++.h>
using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "socket.h"

// 11111111 0000 1111 0000
#define MARKER 0b01111110

typedef struct __attribute__((packed)) message_s {
    unsigned int marker : 8;
    unsigned int size : 6;
    unsigned int seq : 4;
    unsigned int type : 6;
} message;

unsigned int seq = 0;

int main() {
  char* mode = (char*) "lo";
  int soc = ConexaoRawSocket(mode);
  unsigned char buffer[30];

  while(1){
    vector<message> messages;
    for(int i = 0; i < 4; i++){
      message m;
      do{
        read(soc, &buffer, 30);
        memcpy(&m, &buffer, sizeof(message));
      }while(m.marker != MARKER || m.seq != (seq + i) % 16);
      
      messages.push_back(m);
    }

    for(auto m : messages){
      cout << m.seq << '\n';
    }

    cout << '\n';
    seq += 4;
  }

  
  return 0;
}