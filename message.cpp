#include "message.h"

Message* maskToMessage(Mask* ma){
  Message* me = new Message();
  me->marker = ma->marker;
  me->parity = ma->parity;
  me->seq = ma->seq;
  me->size = ma->size;
  me->type = ma->type;
  for(int i = 0; i < 1 << 6; i++){
    me->buff[i] = (unsigned char) ma->buff[i];
  }

  return me;
}
