#include <bits/stdc++.h>
using namespace std;

#include <stdlib.h>
#include "server.h"
#include "message.h"
#include "network.h"

void sendLS(){
  FILE* stream = popen("ls", "r");
  sendStream(::soc, ::serverSeq, ::timedOut, stream, SHOW);
  sendEnd(::soc, ::serverSeq, ::timedOut);
}