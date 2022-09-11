#include <bits/stdc++.h>
using namespace std;

#include <stdlib.h>
#include "server.h"
#include "message.h"
#include "network.h"

void sendStream(FILE* stream, int type){
  while(!feof(stream)){
    Mask *resp = new Mask(SHOW, ::serverSeq);
    int i;
    for(i = 0; !feof(stream) && i < BUFFER_SIZE; i++){
      char c;
      c = fgetc(stream);
      resp->buff[i] = c;
    }
    resp->size = i - 1;

    cout << "[+] enviando LS: " << resp->seq << " " << resp->type << endl;
    sendMask(::soc, resp);
    serverSeq = (serverSeq + 1) % 16;      

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

void sendEnd(){
  Mask *done = new Mask(END, ::serverSeq);

  sendMask(::soc, done);
  serverSeq = (serverSeq + 1) % 16;      

  listenWithTimeout(
      timedOut, 
      soc, 
      done,
      ACK
  );
  delete done;
}