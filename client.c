#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include "socket.h"

int main(){
  int soc = ConexaoRawSocket("lo");

  unsigned char* message = "isso e um teste de vdd";
  while(1){
    sendto(soc, message, 23, 0, NULL, 0);
  }


  return 0;
}