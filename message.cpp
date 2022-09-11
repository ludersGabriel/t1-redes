#include "message.h"
#include "string.h"

Mask_s::Mask_s(){
  memset(&(this->buff), 0l, sizeof(BUFFER_SIZE));
}

Mask_s::Mask_s(int type, int seq){
  this->marker = MARKER;
  this->type = type;
  this->seq = seq;
  memset(&(this->buff), 0l, sizeof(BUFFER_SIZE));
}

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