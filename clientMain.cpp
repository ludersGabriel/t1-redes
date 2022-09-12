#include <bits/stdc++.h>
#include <filesystem>
using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include "socket.h"
#include "message.h"
#include "network.h"
#include "client.h"
#include "utils.h"

#define TIMEOUT 1

void handler(int signum){
  ::timedOut = true;
}

int main(){
  ::soc = ConexaoRawSocket((char*)"enp3s0");
  ::timedOut = false;

  struct pollfd descriptors[2] = {
    {.fd = 0, .events = POLLIN, .revents = 0},
    {.fd = soc, .events = POLLIN, .revents = 0}
  };

  struct sigaction action;
  action.sa_handler = handler;
  sigemptyset (&action.sa_mask) ;
  action.sa_flags = 0 ;
  sigaction (SIGALRM, &action, 0);

  ::clientSeq = 0;
  ::serverSeq = 0;

  ::currentDir = filesystem::current_path();

  system("clear");
  printOptions();
  commandLinePrint("");

  while(1){
    poll(descriptors, 2, -1);

    if(descriptors[1].revents & POLLIN){
      readGarbage(::soc);
    }
    else if(descriptors[0].revents & POLLIN){
      string opt;
      getline(cin, opt);

      string command = opt.substr(0, opt.find(" "));

      if(!opt.compare(::CLIENT_LS)){
        remoteLS();
      }
      else if(!opt.compare(::CLIENT_LS_A)){
        remoteLS("-a");
      }
      else if(!opt.compare(::CLIENT_LS_L)){
        remoteLS("-l");
      }
      else if(!opt.compare(::CLIENT_LS_LA)){
        remoteLS("-la");
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
      else if(!opt.compare(::CLIENT_LSL_LA)){
        system("ls -la");
      }
      else if(!command.compare(::CLIENT_CD)){
        auto ret = getArgs(opt);
        if(!ret.error){
          remoteCD(ret.args);
        }
      }
      else if(!command.compare(::CLIENT_CDL)){
        auto ret = getArgs(opt);
        if(!ret.error){
          localCD(ret.args);
        }
      }
      else if(!command.compare(::CLIENT_MKDIR)){
        auto ret = getArgs(opt);
        if(!ret.error){
          remoteMkdir(ret.args);
        }
      }
      else if(!command.compare(::CLIENT_MKDIRL)){
        auto ret = getArgs(opt);
        if(!ret.error){
          localMkdir(ret.args);
        }
      }
      else if(!opt.compare(::CLIENT_CLEAR)){
        system("clear");
      }
      else if(!opt.compare(::CLIENT_OPTIONS)){
        printOptions();
      }
      else{
        cout << "Comando inválido. Remova qualquer espaço adicional e entre com um comando válido\n" << std::flush;
      }
      commandLinePrint("");
    }

  }


  return 0;
}