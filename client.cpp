#include <unistd.h>
#include <sys/stat.h>
#include "client.h"
#include "network.h"

#include <bits/stdc++.h>
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
  int ret = chdir(args.c_str());

  if(ret == 0){
    ::currentDir = filesystem::current_path();
  }else{
    int codigo = errno;

    switch(codigo){
      case EACCES:
      case EFAULT:
        cout << "Error: faltam permissões\n" << std::flush;
        break;
      case ENOENT:
      case ENOTDIR:
        cout << "Error: diretorio não existe\n" << std::flush;
    }
  }
}

void localMkdir(string args){
  int ret = mkdir(args.c_str(), S_IRWXU);

  if(ret != 0){
    int codigo = errno;

    switch(codigo){
      case EACCES:
      case EFAULT:
        cout << "Error: faltam permissões\n" << std::flush;
        break;
      case EEXIST:
        cout << "Error: diretório ou arquivo já existe\n" << std::flush;
        break;
      case ENOENT:
      case ENOTDIR:
        cout << "Error: caminho não existe\n" << std::flush;
        break;
      default:
        cout << "Error: algo de errado ocorreu\n" << std::flush;
    }
  }
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

    switch((unsigned char) resp->buff[0]){
      case NO_PERM:
        cout << "Error: faltam permissões\n" << std::flush;
        break;
      case NO_DIR:
        cout << "Error: diretorio não existe\n" << std::flush;
        break;
      default:
        cout << "Error: algum erro ocorreu\n" << std::flush;
    }
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
    switch((unsigned char) resp->buff[0]){
      case NO_PERM:
        cout << "Error: faltam permissões\n" << std::flush;
        break;
      case DUP_DIR:
        cout << "Error: diretório já existe\n" << std::flush;
        break;
      case NO_DIR:
        cout << "Error: caminho não existe\n" << std::flush;
        break;
      default:
        cout << "Error: algo de errado ocorreu\n" << std::flush;
    }
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
  filesystem::path p = filesystem::path(args);

  int i = 0;
  for(auto el : p.filename().u8string()){
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
  
  Mask *ack = new Mask(ACK, resp->seq);
  sendMask(soc, ack);
  ::serverSeq = (::serverSeq + 1) % 16;
  ::clientSeq = (::clientSeq + 1) % 16;

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
    if((unsigned char) resp->buff[0] == NO_DIR){
      cout << "error: diretório não encontrado no servidor\n" << std::flush;
    }
    else if((unsigned char) resp->buff[0] == DUP_DIR){
      cout << "error: file já existe no servidor\n" << std::flush;
    }
  }
}

void get(string args){
  filesystem::path p = filesystem::path(args).filename();

  if(filesystem::exists(p)){
    cout << "Error: arquivo já existe nessa máquina\n" << std::flush;
    return;
  }

  Mask* get = new Mask(GET, ::clientSeq);

  int i = 0;
  for(auto el : args){
    get->buff[i] = (unsigned long) el; 
    i++;
  } 
  get->size = i;

  sendMask(::soc, get);
  
  Mask* resp = listenWithTimeout(
    ::timedOut,
    ::soc,
    get,
    ANY,
    ::serverSeq
  );
  
  Mask *ack = new Mask(ACK, resp->seq);
  sendMask(soc, ack);
  ::serverSeq = (::serverSeq + 1) % 16;
  ::clientSeq = (::clientSeq + 1) % 16;


  if(resp->type == ERROR){
    if((unsigned char) resp->buff[0] == NO_FILE)
      cout << "error: arquivo não existe no servidor\n" << std::flush;
    else{
      cout << "error: algum erro ocorreu\n" << std::flush;
    }
    return;
  }else if(resp->type == OK){
    FILE* f = fopen(&(p.u8string()[0]), "wb");
    
    consumeStream(
      ::soc,
      ::serverSeq,
      ::timedOut,
      DATA,
      ack,
      f
    );

    fclose(f);
  }

}