#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, const char* argv[]){
  if (argc != 3) {
    fprintf(stderr, "Incorrect number of arguments.");
    exit(1);
  }
  
  const char* from = argv[1];
  const char* to = argv[2];
  
  if (strlen(from) != strlen(to)) {
    fprintf(stderr, "Unequal number of letters.");
    exit(1);
  }

  int i = 0;
  int j = 0;

  for (i = 0; i < strlen(from); i++) {
    for (j = i + 1; j < strlen(from); j++) {
      if (from[i] == from[j]){
	fprintf(stderr, "Repeats Detected.");
	exit(1);
      }
    }
  }

  while (!feof(stdin)){
    int input = getchar();
    
    if (ferror(stdin)) {
      fprintf(stderr, "Unable to read input.");
      exit(1);
    }
    
    if (input == EOF) {
      break;
    }

    bool translate = false;

    for (i = 0; i < strlen(from); i++){
      if (input == from[i]) {
	putchar(to[i]);
	if (ferror(stdout)){
	  fprintf(stderr, "Unable to write output.");
	  exit(1);
	}
	translate = true;
	break;
      }
    }

    if (translate == false) {
      putchar(input);
      if (ferror(stdout)) {
	fprintf(stderr, "Unable to write output.");
	exit(1);
      }
    }

  }
  exit(0);
}
