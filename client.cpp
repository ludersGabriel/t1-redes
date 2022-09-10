#include "client.h"
#include "network.h"

using namespace std;

void commandLinePrint(string out){
  cout << currentDir << ": " << out;
}

void printOptions(){
  cout << "#### COMANDOS ####\n";
  for(auto el : ::clientCommands){
    cout << '\t' << el << '\n';
  }
  cout << "obs: comandos com 'l' no final são locais\n\n";
}

void remoteLS(){
  Mask* lsMask = new Mask();
  lsMask->type = LS;
  lsMask->marker = MARKER;
  lsMask->seq = ::clientSeq;

  sendMask(::soc, lsMask);
  
  
  Mask* ma = NULL;
  ma = listenWithTimeout(
      ::timedOut,
      ::soc,
      lsMask,
      ANY
    );

  while(ma->type == SHOW){
    Message* m = maskToMessage(ma);

    for(int i = 0; i < ma->size; i++){
      cout << m->buff[i];
    }

    Mask *ack = new Mask();
    ack->type = ACK;
    ack->marker = MARKER;
    ack->seq = m->seq;
    sendMask(::soc, ack);
    ::serverSeq = (serverSeq + 1) % 16;

    delete ma;
    ma = listenType(::soc, ANY);
  }

  if(ma) delete ma;

  ::clientSeq = (::clientSeq + 1) % 16;

}