#include <bits/stdc++.h>
using namespace std;

#include "message.h"
#include "string.h"

Mask_s::Mask_s(){
  memset(&(this->buff), 0l, sizeof(unsigned long) * (BUFFER_SIZE));
}

Mask_s::Mask_s(int type, int seq){
  this->marker = MARKER;
  this->size = 0;
  this->seq = seq;
  this->type = type;
  memset(&(this->buff), 0l, sizeof(unsigned long) * (BUFFER_SIZE));
  this->parity = 0;
}

Mask_s::Mask_s(int type, int seq, int size, unsigned char buffer[32]){
  this->marker = MARKER;
  this->size = size;
  this->seq = seq;
  this->type = type;

  for(int i = 0; i < size; i++){
    this->buff[i] = (unsigned long) buffer[i];
  }

  this->parity = 0;
}

void setParity(Mask* mask){
  mask->parity = 0;
  for(int i = 0; i <= mask->size; i++){
    mask->parity ^= (unsigned char) mask->buff[i];
  }
}

bool checkParity(Mask* mask){
  unsigned char p = 0;
  for(int i = 0; i <= mask->size; i++){
    p ^= (unsigned char) mask->buff[i];
  }

  return p == mask->parity;
}

Message* maskToMessage(Mask* ma){
  Message* me = new Message();
  me->marker = ma->marker;
  me->parity = ma->parity;
  me->seq = ma->seq;
  me->size = ma->size;
  me->type = ma->type;
  for(int i = 0; i <= 63; i++){
    me->buff[i] = (unsigned char) ma->buff[i];
  }

  return me;
}