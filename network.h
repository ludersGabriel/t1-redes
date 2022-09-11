#ifndef NETWORK_HEADER
#define NEWTWOK_HEADER

#include <stdlib.h>
#include "message.h"
#define TIMEOUT 1

Mask* listenWithTimeout(
  bool& timedOut,
  int soc,
  Mask* resend,
  int type
);

Mask* listenType(int soc, int type);

void sendMask(int soc, Mask* mask);

void sendStream(int soc, long& seq, bool& timedOut, FILE* stream, int type);

void sendEnd(int soc, long& seq, bool& timedOut);

#endif