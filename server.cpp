#include <bits/stdc++.h>
#include <filesystem>
using namespace std;

#include <stdlib.h>
#include <unistd.h>
#include "server.h"
#include "message.h"
#include "network.h"

void sendLS(string ls){
  FILE* stream = popen(&ls[0], "r");

  sendStream(::soc, ::serverSeq, ::timedOut, stream, SHOW);
  sendEnd(::soc, ::serverSeq, ::timedOut);

  fclose(stream);
  clientSeq = (clientSeq + 1) % 16;
}

void resolvePUT(string path){
  FILE* f = fopen(&path[0], "wb");

  if(!f){
    Mask* ma = new Mask(ERROR, ::serverSeq);
    ma->buff[0] = (unsigned long) NO_DIR;
    sendMask(soc, ma);
    
    Mask* ack = listenWithTimeout(
      ::timedOut, 
      ::soc, 
      ma,
      ACK,
      ::serverSeq
    );

    serverSeq = (serverSeq + 1) % 16;
    delete ma;
    delete ack;
    clientSeq = (clientSeq + 1) % 16;
    return;
  }

  Mask* ma = new Mask(OK, ::serverSeq);
  sendMask(soc, ma);

  Mask* ack = listenWithTimeout(
    ::timedOut, 
    ::soc, 
    ma,
    ACK,
    ::serverSeq
  );

  serverSeq = (serverSeq + 1) % 16;
  delete ma;
  delete ack;
  clientSeq = (clientSeq + 1) % 16;

  consumeStream(
    ::soc,
    ::clientSeq,
    ::timedOut,
    DATA,
    ma,
    f
  );

  fclose(f);
}

void resolveGET(string path){
  if(!filesystem::exists(path) || filesystem::is_directory(path)){
    Mask* ma = new Mask(ERROR, ::serverSeq);
    ma->buff[0] = (unsigned long) NO_DIR;
    sendMask(soc, ma);
    
    Mask* ack = listenWithTimeout(
      ::timedOut, 
      ::soc, 
      ma,
      ACK,
      ::serverSeq
    );

    serverSeq = (serverSeq + 1) % 16;
    delete ma;
    delete ack;
    clientSeq = (clientSeq + 1) % 16;
    return;
  }

  Mask* ma = new Mask(OK, ::serverSeq);
  sendMask(soc, ma);
  
  Mask* ack = listenWithTimeout(
    ::timedOut, 
    ::soc, 
    ma,
    ACK,
    ::serverSeq
  );

  serverSeq = (serverSeq + 1) % 16;
  delete ma;
  delete ack;
  clientSeq = (clientSeq + 1) % 16;

  FILE* f = fopen(&path[0], "rb");
  sendStream(
    ::soc,
    ::serverSeq,
    ::timedOut,
    f,
    DATA
  );
  sendEnd(soc, ::serverSeq, ::timedOut);
  fclose(f);
}

void resolveCD(string path){ 
  int ret = chdir(path.c_str());

  if(ret != 0){
    Mask *resp = new Mask(ERROR, ::serverSeq);
    int codigo = errno;

    switch (codigo){
    case EACCES:
    case EFAULT:
      resp->buff[0] = (unsigned long) NO_PERM; 
      break;
    case ENOENT:
    case ENOTDIR:
      resp->buff[0] = (unsigned long) NO_DIR;
    default:
      break;
    }

    cout << "[+] sent CD: " << resp->seq << " " << resp->type << " " << resp->size << endl << endl << std::flush;

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
    clientSeq = (clientSeq + 1) % 16;
    return;
  }

  ::currentDir = filesystem::current_path(); 

  Mask *resp = new Mask(OK, ::serverSeq);
  
  cout << "[+] sent CD: " << resp->seq << " " << resp->type << " " << resp->size << endl << endl << std::flush;
  sendMask(::soc, resp);

  Mask* ack = listenWithTimeout(
      ::timedOut, 
      ::soc, 
      resp,
      ACK,
      ::serverSeq
    );

  serverSeq = (serverSeq + 1) % 16;
  clientSeq = (clientSeq + 1) % 16;
  delete resp;
  delete ack;
}

void resolveMkdir(string path){ 
  if(filesystem::exists(path)){
    Mask *resp = new Mask(ERROR, ::serverSeq);
    resp->buff[0] = (unsigned long) DUP_DIR;
    
    cout << "[+] sent MKDIR: " << resp->seq << " " << resp->type << " " << resp->size << endl << endl << std::flush;
    sendMask(::soc, resp);

    Mask* ack = listenWithTimeout(
      ::timedOut, 
      ::soc, 
      resp,
      ACK,
      ::serverSeq
    );

    serverSeq = (serverSeq + 1) % 16;
    clientSeq = (clientSeq + 1) % 16;
    delete resp;
    delete ack;
    return;
  }

  filesystem::create_directories(path);

  Mask *resp = new Mask(OK, ::serverSeq);
  
  cout << "[+] sent CD: " << resp->seq << " " << resp->type << " " << resp->size << endl << endl << std::flush;
  sendMask(::soc, resp);

  Mask* ack = listenWithTimeout(
      ::timedOut, 
      ::soc, 
      resp,
      ACK,
      ::serverSeq
    );

  serverSeq = (serverSeq + 1) % 16;
  clientSeq = (clientSeq + 1) % 16;

  delete resp;
  delete ack;
  
}
