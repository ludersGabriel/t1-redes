#ifndef CLIENT_HEADER
#define CLIENT_HEADER

#include <bits/stdc++.h>
#include <filesystem>
using namespace std;

inline filesystem::path currentDir;
inline string CLIENT_LS = "ls";
inline string CLIENT_LS_A = "ls -a";
inline string CLIENT_LS_L = "ls -l";
inline string CLIENT_LSL = "lsl";
inline string CLIENT_LSL_A = "lsl -a";
inline string CLIENT_LSL_L = "lsl -l";
inline string CLIENT_LSL_LA = "lsl -la";
inline string CLIENT_CLEAR = "clear";
inline string CLIENT_OPTIONS = "options";
inline string CLIENT_LS_LA = "ls -la";
inline string CLIENT_CD = "cd";
inline string CLIENT_CDL = "cdl";
inline string CLIENT_MKDIR = "mkdir";
inline string CLIENT_MKDIRL = "mkdirl";


inline vector<string> clientCommands = {
  CLIENT_LS,
  CLIENT_LS_A,
  CLIENT_LS_L,
  CLIENT_LS_LA,
  CLIENT_LSL,
  CLIENT_LSL_A,
  CLIENT_LSL_L,
  CLIENT_LSL_LA,
  CLIENT_CLEAR,
  CLIENT_OPTIONS,
  CLIENT_CD,
  CLIENT_CDL,
  CLIENT_MKDIR,
  CLIENT_MKDIRL
};

inline int soc;
inline bool timedOut;
inline long clientSeq;
inline long serverSeq;

void commandLinePrint(string out);

void printOptions();

void remoteLS(string s = "");
void localCD(string args);
void localMkdir(string args);

#endif