#ifndef UTILS_H
#define UTILS_H

struct ArgsRet {
  bool error;
  string args;
};

ArgsRet getArgs(string opt);

#endif