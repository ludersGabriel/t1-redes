#ifndef CLIENT_HEADER
#define CLIENT_HEADER

#include <bits/stdc++.h>

using namespace std;

inline string currentDir;
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
  CLIENT_OPTIONS
};

inline int soc;
inline bool timedOut;
inline long clientSeq;
inline long serverSeq;

void commandLinePrint(string out);

void printOptions();

void remoteLS(string s = "");

#endif