#include <unistd.h>
#include <sys/socket.h>
#include "network.h"
#include "message.h"

#include <bits/stdc++.h>
using namespace std;

Mask* listenWithTimeout(
  bool& timedOut,
  int soc,
  Mask* resend,
  int type
){
  Mask* ma = new Mask();
  int acked;
  acked = false; 
  do{
      timedOut = false;
        
      alarm(TIMEOUT);
      do{
        recv(soc, ma, sizeof(Mask), 0);
      }while(
        !timedOut
        && (ma->marker != MARKER || (type != ANY && ma->type != type))
      );
      alarm(0);

      if(timedOut){
        write(soc, resend, sizeof(Mask));
        continue;
      }

      if(ma->type == NACK){
        write(soc, resend, sizeof(Mask));
      }
      else if(ma->type == ACK){
        acked = true;
      }

    }while(timedOut || !acked);

    return ma;
}

Mask* listenType(int soc, int type){
  Mask* ma = new Mask;

  do{
    recv(soc, ma, sizeof(Mask), 0);
  }while(
    ma->marker != MARKER 
    || (type != ANY && ma->type != type)
  );

  return ma;
}