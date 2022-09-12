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

void remoteLS(string s){
  Mask* lsMask = new Mask(LS, ::clientSeq);

  int i = 0;
  for(auto el : s){
    lsMask->buff[i] = (unsigned long) el; 
    i++;
  } 
  lsMask->size = i;

  setParity(lsMask);
  sendMask(::soc, lsMask);
  
  consumeStream(
    ::soc,
    ::serverSeq,
    ::timedOut,
    ANY,
    lsMask,
    stdout
  );

  ::clientSeq = (::clientSeq + 1) % 16;

}