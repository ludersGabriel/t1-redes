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
        cout << "ack\n";
      }

    }while(timedOut || !acked);

    return ma;
}

Mask* listenType(int soc, int type){
  Mask* ma = new Mask();

  do{
    recv(soc, ma, sizeof(Mask), 0);
  }while(ma->marker != MARKER);

  return ma;
}

Mask* listenResend(int soc, int type, long& seq, Mask* resend){
  Mask* ma = NULL;
  do{
    if(ma) delete ma;
    ma = listenType(soc, type);

    if(ma->seq < seq){
      sendMask(soc, resend);
    }
    
  }while(ma->seq < seq);

  return ma;
}

void sendMask(int soc, Mask* mask){
  write(soc, mask, sizeof(Mask));
}

void sendStream(int soc, long& seq, bool& timedOut, FILE* stream, int type){
  while(!feof(stream)){
    Mask *resp = new Mask(SHOW, seq);

    unsigned int i;
    unsigned char c;
    for(i = 0; !feof(stream) && i < BUFFER_SIZE; i++){
      c = fgetc(stream);
      resp->buff[i] = (unsigned long) c;
    }
    resp->size = i - 1;

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
}

void consumeStream(int soc, long& seq, bool& timedOut, int type, Mask* resend, FILE* file){
  Mask* ma = NULL;
  ma = listenWithTimeout(
      timedOut,
      soc,
      resend,
      ANY
    );

  while(ma->type == SHOW || ma->type == DATA){
    Message* m = maskToMessage(ma);
    
    for(int i = 0; i <= m->size; i++){
      fprintf(file, "%c", m->buff[i]);
    }

    Mask *ack = new Mask(ACK, m->seq);
    
    int chance = rand() % 3;

    sendMask(soc, ack);
    seq = (seq + 1) % 16;

    delete ma;
    ma = listenResend(soc, ANY, seq, ack);
    delete(ack);
  }

  if(ma->type == END){
    Mask *ack = new Mask(ACK, ma->seq);
    sendMask(soc, ack);
    seq = (seq + 1) % 16;
    delete ack;
  }

  if(ma) delete ma;
}

void sendEnd(int soc, long& seq, bool& timedOut){
  Mask *done = new Mask(END, seq);

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