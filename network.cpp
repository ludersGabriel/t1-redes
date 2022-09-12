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
  int type,
  long seq
){
  Mask* ma = new Mask();
  bool acked = type != ACK;
  bool nacked = false;
  bool seen;

  do{
      timedOut = false;
      acked = type != ACK;
      nacked = false;
      seen = false;
        
      alarm(TIMEOUT);
      do{
        recv(soc, ma, sizeof(Mask), 0);
      }while(
        !timedOut
        && (ma->marker != MARKER)
      );
      alarm(0);

      if(timedOut){
        sendMask(soc, resend);
        timedOut = true;
        continue;
      }

      if(ma->type != ACK && ma->type != NACK && ma->seq < seq){
        seen = true;
        sendMask(soc, resend);
        continue;
      }

      if(!checkParity(ma)){
        Mask* nack = new Mask(NACK, ma->seq);
        sendMask(soc, nack);
        nacked = true;
        continue;
      }

      if(ma->type == NACK){
        sendMask(soc, resend);
        nacked = true;
        continue;
      }
      else if(ma->type == ACK){
        acked = true;
        // cout << "ack\n";
      }

    }while(timedOut || !acked || nacked || seen);

    return ma;
}

Mask* listenType(int soc, int type){
  Mask* ma = new Mask();
  bool nacked = false;
  do{
    nacked = false;

    do{
      recv(soc, ma, sizeof(Mask), 0);
    }while(ma->marker != MARKER);

    if(!checkParity(ma)){
      cout << "[-] parity error: " << ma->seq << endl << std::flush;
      Mask* nack = new Mask(NACK, ma->seq);
      sendMask(soc, nack);
      nacked = true;
      continue;
    }

  }while(nacked);

  return ma;
}

void readGarbage(int soc){
  Mask* ma = new Mask();

  recv(soc, ma, sizeof(Mask), 0);

  if(ma->marker != MARKER) return;

  if(!checkParity(ma)){
    cout << "[-] parity error: " << ma->seq << endl << std::flush;
    Mask* nack = new Mask(NACK, ma->seq);
    sendMask(soc, nack);

    return;
  }

  Mask* ack = new Mask(ACK, ma->seq);
  sendMask(soc, ack);
  delete ma;
  delete ack;
}

void sendMask(int soc, Mask* mask){
  setParity(mask);
  cout << "sending m: " << mask->seq << endl << std::flush;
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


    cout << "[+] sent LS: " << resp->seq << " " << resp->type << " " << resp->size << endl << std::flush;
    sendMask(soc, resp);

    Mask* ack = listenWithTimeout(
      timedOut, 
      soc, 
      resp,
      ACK,
      seq
    );

    delete ack;
    delete resp;
    seq = (seq + 1) % 16;      
  }
  cout << '\n';
}

void consumeStream(int soc, long& seq, bool& timedOut, int type, Mask* resend, FILE* file){
  cout << "[+] listening for stream" << endl << std::flush;
  Mask* ma = NULL;
  ma = listenWithTimeout(
      timedOut,
      soc,
      resend,
      ANY,
      seq
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
    ma = listenWithTimeout(
      timedOut,
      soc,
      ack,
      ANY,
      seq
    );
    cout << std::flush;
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


  cout << "[+] sent End: " << done->seq << " " << done->type << " " << done->size << "\n\n" << std::flush;

  sendMask(soc, done);
  seq = (seq + 1) % 16;      

  listenWithTimeout(
      timedOut, 
      soc, 
      done,
      ACK,
      seq
  );
  
  delete done;
}