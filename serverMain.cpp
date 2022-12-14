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

void handler(int signum){
  ::timedOut = true;
}

int main(int argc, char** argv) {
  if(argc < 2){
    cout << "Usage: ./server placa\n" << std::flush;
    return 1;
  }

  char* mode = (char*) argv[1];
  ::soc = ConexaoRawSocket(mode);

  struct sigaction action;
  action.sa_handler = handler;
  sigemptyset (&action.sa_mask) ;
  action.sa_flags = 0 ;
  sigaction (SIGALRM, &action, 0);

  ::clientSeq = 0;
  ::serverSeq = 0;

  currentDir = filesystem::current_path();

  cout << "Atualmente em: " << currentDir << endl << std::flush;
  while(1){
    Mask* ma = listenType(soc, ANY, ::clientSeq);

    cout << "RECEIVED: " << ma->seq << ", EXPECTED: " << ::clientSeq << endl << std::flush;

    Message* recMe = maskToMessage(ma);

    switch (recMe->type){
      case LS: {
        cout << "[+] received LS: " << recMe->seq << endl << std::flush;
        string ls = "ls ";
        for(int i = 0; i < recMe->size; i++)
          ls += recMe->buff[i];
        sendLS(ls);
        break;
      }
      case CD: {
        cout << "[+] received CD: " << recMe->seq << endl << std::flush;
        string path = "";
        for(int i = 0; i < recMe->size; i++)
          path += recMe->buff[i];
        resolveCD(path);
        break;
      }
      case MKDIR: {
        cout << "[+] received MKDIR: " << recMe->seq << endl << std::flush;
        string path = "";
        for(int i = 0; i < recMe->size; i++)
          path += recMe->buff[i];
        resolveMkdir(path);
        break;
      }
      case PUT: {
        cout << "[+] received PUT: " << recMe->seq << endl << std::flush;
        string path = "";
        for(int i = 0; i < recMe->size; i++)
          path += recMe->buff[i];
        resolvePUT(path);
        break;
      }
      case GET: {
        cout << "[+] received GET: " << recMe->seq << endl << std::flush;
        string path = "";
        for(int i = 0; i < recMe->size; i++)
          path += recMe->buff[i];
        resolveGET(path);
        break;
      }
      default:
        break;
    }
    
    delete ma;
    delete recMe;
    cout << "Atualmente em: " << currentDir << endl << std::flush;

  }

  
  return 0;
}