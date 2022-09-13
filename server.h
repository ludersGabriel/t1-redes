#ifndef SERVER_HEADER
#define SERVER_HEADER

#include <bits/stdc++.h>
#include <filesystem>
using namespace std;

inline filesystem::path currentDir;
inline long clientSeq;
inline long serverSeq;
inline bool timedOut;
inline int soc;

void sendLS(string ls);
void resolveCD(string path);
void resolveMkdir(string path);
void resolvePUT(string path);
void resolveGET(string path);


#endif