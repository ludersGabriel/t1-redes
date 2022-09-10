#include "message.h"
#include "string.h"

Mask_s::Mask_s(){
  memset(&(this->buff), ' ', sizeof(BUFFER_SIZE));
}

Mask_s::Mask_s(int type, int seq){
  this->marker = MARKER;
  this->type = type;
  this->seq = seq;
  memset(&(this->buff), ' ', sizeof(BUFFER_SIZE));
}