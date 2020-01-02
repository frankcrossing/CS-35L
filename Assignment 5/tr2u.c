#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, const char* argv[]){
  if (argc != 3) {
    write(2, "Incorrect number of arguments.", 30);
    exit(1);
  }
  
  const char* from = argv[1];
  const char* to = argv[2];
  
  if (strlen(from) != strlen(to)) {
    write(2, "Unequal number of letters.", 26);
    exit(1);
  }

  int i = 0;
  int j = 0;

  for (i = 0; i < strlen(from); i++) {
    for (j = i + 1; j < strlen(from); j++) {
      if (from[i] == from[j]){
	write(2, "Repeats Detected.", 17);
	exit(1);
      }
    }
  }

  char input[1];

  while (read(0, input, 1) > 0){
    
    bool translate = false;

    for (i = 0; i < strlen(from); i++){
      if (input[0] == from[i]) {
	char output[1];
	output[0] = to[i];
	if (write(1, output, 1) < 0){
	  write(2, "Error with output.", 18);
	  exit(1);
	}
      translate = true;
      break;
      }
    }

    if (translate == false) {
      if (write(1, input, 1) < 0) {
	write(2, "Error with output.", 18);
	exit(1);
      }
    }

  }
  exit(0);
}
