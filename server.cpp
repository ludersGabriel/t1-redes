#include <bits/stdc++.h>
using namespace std;

#include <stdlib.h>
#include "server.h"
#include "message.h"
#include "network.h"

void sendLS(string ls){
  FILE* stream = popen(&ls[0], "r");

  fseek(stream, 0, SEEK_SET);
  sendStream(::soc, ::serverSeq, ::timedOut, stream, SHOW);
  sendEnd(::soc, ::serverSeq, ::timedOut);

  fclose(stream);
}