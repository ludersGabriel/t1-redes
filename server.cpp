#include <bits/stdc++.h>
#include <filesystem>
using namespace std;

#include <stdlib.h>
#include "server.h"
#include "message.h"
#include "network.h"

void sendLS(string ls){
  FILE* stream = popen(&ls[0], "r");

  sendStream(::soc, ::serverSeq, ::timedOut, stream, SHOW);
  sendEnd(::soc, ::serverSeq, ::timedOut);

  fclose(stream);
}

void resolveCD(string path){  
  if(!filesystem::is_directory(path)){
    Mask *resp = new Mask(ERROR, ::serverSeq);
    resp->buff[0] = (unsigned long) 'a';
    
    setParity(resp);
    sendMask(::soc, resp);

    Mask* ack = listenWithTimeout(
      ::timedOut, 
      ::soc, 
      resp,
      ACK,
      ::serverSeq
    );

    serverSeq = (serverSeq + 1) % 16;
    delete resp;
    delete ack;
    return;
  }

  filesystem::current_path(path);
  ::currentDir = filesystem::current_path(); 

  Mask *resp = new Mask(OK, ::serverSeq);
  
  setParity(resp);
  sendMask(::soc, resp);

  Mask* ack = listenWithTimeout(
      ::timedOut, 
      ::soc, 
      resp,
      ACK,
      ::serverSeq
    );

  serverSeq = (serverSeq + 1) % 16;

  delete resp;
  delete ack;
}
