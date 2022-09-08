#ifndef NETWORK_HEADER
#define NEWTWOK_HEADER

#include "message.h"
#define TIMEOUT 1

Mask* listenWithTimeout(
  bool& timedOut,
  int soc,
  Mask* resend,
  int type
);

Mask* listenType(int soc, int type);

#endif