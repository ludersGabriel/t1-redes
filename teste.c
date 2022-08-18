#include <stdio.h>
#include <unistd.h>

int main() {

  for(int i = 0; ;i = (i + 1) % 16){
    printf("%d\n", i);
    sleep(1);
  }

  return 0;
}