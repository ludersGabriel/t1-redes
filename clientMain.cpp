#include <bits/stdc++.h>
#include <filesystem>
using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "socket.h"
#include "message.h"
#include "network.h"
#include "client.h"

#define TIMEOUT 1

void handler(int signum){
  ::timedOut = true;
}

int main(){
  ::soc = ConexaoRawSocket((char*)"enp3s0");
  ::timedOut = false;

  struct sigaction action;
  action.sa_handler = handler;
  sigemptyset (&action.sa_mask) ;
  action.sa_flags = 0 ;
  sigaction (SIGALRM, &action, 0);

  ::clientSeq = 0;
  ::serverSeq = 0;

  char *s = (char*)"batata";
  ::currentDir = filesystem::current_path();
  system("clear");
  printOptions();

  while(1){
    commandLinePrint("");
    string opt;
    getline(cin, opt); 

    if(!opt.compare(::CLIENT_LS)){
      remoteLS();
    }
    else if(!opt.compare(::CLIENT_LS_A)){
      commandLinePrint("ls -a\n");
    }
    else if(!opt.compare(::CLIENT_LS_L)){
      commandLinePrint("ls -l\n");
    }
    else if(!opt.compare(::CLIENT_LSL)){
      system("ls");
    }
    else if(!opt.compare(::CLIENT_LSL_A)){
      system("ls -a");
    }
    else if(!opt.compare(::CLIENT_LSL_L)){
      system("ls -l");
    }
    else if(!opt.compare(::CLIENT_CLEAR)){
      system("clear");
    }
    else if(!opt.compare(::CLIENT_OPTIONS)){
      cout << '\n';
      printOptions();
    }
    else{
      commandLinePrint("default\n");
    }

    cout << "\n";
  }


  return 0;
}