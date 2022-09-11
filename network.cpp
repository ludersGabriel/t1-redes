#include <sys/socket.h>
#include <bits/stdc++.h>
using namespace std;

#include <stdlib.h>
#include <unistd.h>
#include "network.h"
#include "message.h"

Mask* listenWithTimeout(
  bool& timedOut,
  int soc,
  Mask* resend,
  int type
){
  Mask* ma = new Mask();
  int acked;
  acked = type != ACK; 
  do{
      timedOut = false;
        
      alarm(TIMEOUT);
      do{
        recv(soc, ma, sizeof(Mask), 0);
      }while(
        !timedOut
        && (ma->marker != MARKER)
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
        // cout << "ack\n";
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
  );

  return ma;
}

void sendMask(int soc, Mask* mask){
  write(soc, mask, sizeof(Mask));
}

void sendStream(int soc, long& seq, bool& timedOut, FILE* stream, int type){
  while(!feof(stream)){
    Mask *resp = new Mask(SHOW, seq);

    unsigned int i = 0;
    unsigned char c;

    c = fgetc(stream);
    for(i = 0; !feof(stream) && i < (BUFFER_SIZE) - 1; i++){
      resp->buff[i] = (unsigned long) c;
      c = fgetc(stream);
    }

    if(!feof(stream)){
      resp->buff[(BUFFER_SIZE) - 1] = c;
      resp->size = i;
    }
    else resp->size = i - 1;


    cout << "[+] enviando LS: " << resp->seq << " " << resp->type << " " << resp->size << endl;
    sendMask(soc, resp);
    seq = (seq + 1) % 16;      

    Mask* ack = listenWithTimeout(
      timedOut, 
      soc, 
      resp,
      ACK
    );

    delete ack;
    delete resp;
  }
  cout << '\n';
}

void sendEnd(int soc, long& seq, bool& timedOut){
  Mask *done = new Mask(END, seq);


  cout << "[+] enviando End: " << done->seq << " " << done->type << " " << done->size << endl << endl;

  sendMask(soc, done);
  seq = (seq + 1) % 16;      

  listenWithTimeout(
      timedOut, 
      soc, 
      done,
      ACK
  );
  delete done;
}