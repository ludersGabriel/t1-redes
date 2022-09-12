#include "client.h"
#include "network.h"

using namespace std;

void commandLinePrint(string out){
  cout << ::currentDir << ": " << out << std::flush;
}

void printOptions(){
  cout << "#### COMANDOS ####\n" << std::flush;;
  for(auto el : ::clientCommands){
    cout << '\t' << el << '\n' << std::flush;
  }
  cout << "obs: comandos com 'l' no final são locais\n\n" << std::flush;
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

void localCD(string args){
  if(!filesystem::is_directory(args)){
    cout << "Error: diretório não existe\n" << std::flush;
    return;
  }
  
  filesystem::current_path(args);
  ::currentDir = filesystem::current_path();
}

void localMkdir(string args){
  if(filesystem::exists(args)){
    cout << "Error: diretório ou arquivo já existe\n" << std::flush;
  }
  
  filesystem::create_directory(args);
}