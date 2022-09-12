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

Mask* listenResend(int soc, int type, long& seq, Mask* resend);

void sendMask(int soc, Mask* mask);

void sendStream(int soc, long& seq, bool& timedOut, FILE* stream, int type);

void sendEnd(int soc, long& seq, bool& timedOut);

void consumeStream(int soc, long& seq, bool& timedOut, int type, Mask* resend, FILE* buffer);

#endif