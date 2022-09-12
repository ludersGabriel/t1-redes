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
  
  filesystem::create_directories(args);
}

void remoteCD(string args){
  Mask* cd = new Mask(CD, ::clientSeq);

  int i = 0;
  for(auto el : args){
    cd->buff[i] = (unsigned long) el; 
    i++;
  } 
  cd->size = i;

  sendMask(::soc, cd);

  Mask* resp = listenWithTimeout(
    ::timedOut,
    ::soc,
    cd,
    ANY,
    ::serverSeq
  );

  Mask *ack = new Mask(ACK, resp->seq);
  sendMask(soc, ack);
  ::serverSeq = (::serverSeq + 1) % 16;

  ::clientSeq = (::clientSeq + 1) % 16;

  if(resp->type == ERROR){
    if((unsigned char) resp->buff[0] == NO_DIR)
      cout << "error: diretório não existe\n" << std::flush;
    else{
      cout << "error: algum erro ocorreu\n" << std::flush;
    }
    return;
  }

}

void remoteMkdir(string args){
  Mask* mkdir = new Mask(MKDIR, ::clientSeq);

  int i = 0;
  for(auto el : args){
    mkdir->buff[i] = (unsigned long) el; 
    i++;
  } 
  mkdir->size = i;

  sendMask(::soc, mkdir);

  Mask* resp = listenWithTimeout(
    ::timedOut,
    ::soc,
    mkdir,
    ANY,
    ::serverSeq
  );

  Mask *ack = new Mask(ACK, resp->seq);
  sendMask(soc, ack);
  ::serverSeq = (::serverSeq + 1) % 16;

  ::clientSeq = (::clientSeq + 1) % 16;

  if(resp->type == ERROR){
    if((unsigned char) resp->buff[0] == DUP_DIR)
      cout << "error: diretório ou arquivo já existe\n" << std::flush;
    else{
      cout << "error: algum erro ocorreu\n" << std::flush;
    }
    return;
  }
}

void put(string args){
  if(!filesystem::exists(args)){
    cout << "error: arquivo não existe\n" << std::flush;
    return;
  }

  if(filesystem::is_directory(args)){
    cout << "error: não enviamos diretórios\n" << std::flush;
    return;
  }

  Mask* put = new Mask(PUT, ::clientSeq);

  int i = 0;
  for(auto el : args){
    put->buff[i] = (unsigned long) el; 
    i++;
  } 
  put->size = i;

  sendMask(::soc, put);

  Mask* resp = listenWithTimeout(
    ::timedOut,
    ::soc,
    put,
    ANY,
    ::serverSeq
  );
  
  ::serverSeq = (::serverSeq + 1) % 16;

  if(resp->type == OK){
    FILE* f = fopen(&args[0], "rb");
    
    sendStream(
      ::soc,
      ::clientSeq,
      ::timedOut,
      f,
      DATA
    );
    
    sendEnd(::soc, ::clientSeq, ::timedOut);

    fclose(f);
  }else{

  }
}