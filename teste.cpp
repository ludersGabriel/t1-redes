#include <bits/stdc++.h>
using namespace std;

#include <stdio.h>
#include <unistd.h>

int main() {
  int counter = 0;

  for(;;counter += 3){
    for(int i = 0; i < 3; i++){
      cout << (counter + i) % 16 << '\n';
    }

    cout << '\n';
    sleep(1);
  }

  return 0;
}