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
  Mask* lsMask = new Mask(LS, ::clientSeq);
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
    
    for(int i = 0; i <= m->size; i++){
      cout << m->buff[i];
    }

    Mask *ack = new Mask(ACK, m->seq);
    
    int chance = rand() % 3;
    // if(!(chance == 1))
      sendMask(::soc, ack);
    ::serverSeq = (serverSeq + 1) % 16;

    delete ma;
    ma = listenResend(::soc, ANY, ::serverSeq, ack);
    delete(ack);
  }

  if(ma->type == END){
    Mask *ack = new Mask();
    ack->type = ACK;
    ack->marker = MARKER;
    ack->seq = ma->seq;
    sendMask(::soc, ack);
    ::serverSeq = (serverSeq + 1) % 16;
    delete ack;
  }

  if(ma) delete ma;

  ::clientSeq = (::clientSeq + 1) % 16;

}