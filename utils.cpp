#include <bits/stdc++.h>
#include <filesystem>
using namespace std;

#include "utils.h"



struct ArgsRet getArgs(string opt){
  int index = opt.find(" ");

  struct ArgsRet ret {.error = true, .args = ""};

  if(index == -1 || (unsigned int) index == string::npos){
    cout << "Error: nenhum argumento passado\n" << std::flush;
    return ret;
  }else{
    string args = opt.substr(index + 1); 
    if(!args.compare(" ") || !args.compare("")){
      cout << "Error: Nenhum argumento passado\n" << std::flush;
      return ret;
    }
    else{
      ret.args = args;
      ret.error = false;
      return ret;
    }
  }

  return ret;
}