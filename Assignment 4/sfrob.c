#include <stdio.h>
#include <stdlib.h>

int frobcmp(const char* a, const char* b){

  while (*a != ' ' || *b != ' '){

    char realA = (*a)^(0x2a);
    char realB = (*b)^(0x2a);

    if (realA > realB || *b == ' '){
      return 1;
    }

    else if (realB > realA || *a == ' '){
      return -1;
    }

    a++;
    b++;
  }

  return 0;
}

int compare(const void* a, const void* b) {
  return ( frobcmp(*(char**)a, *(char**)b) );
}

int main() {

  char** stringStorage = (char**)malloc(sizeof(char*));

  if (stringStorage == NULL) {
    fprintf(stderr, "Error allocating memory.");
    exit(1);
  }

  char* string = (char*)malloc(sizeof(char));

  if (string == NULL) {
    fprintf(stderr, "Error allocating memory.");
    exit(1);
  }

  int numberOfChars = 0;
  int words = 0;

  while(!feof(stdin)) {

    int input = getchar();

    if (ferror(stdin)){
      fprintf(stderr, "Error reading input.");
      exit(1);
    }

    numberOfChars++;

    string = (char*)realloc(string, numberOfChars*sizeof(char));

    if (string == NULL) {
      fprintf(stderr, "Error allocating memory.");
      exit(1);
    }

    if (input == EOF || feof(stdin)) {
      string[numberOfChars - 1] = ' ';
      stringStorage[words] = string;
      numberOfChars = 0;
      words++;
      break;
    }
    if (input == ' ') {
      string[numberOfChars - 1] = input;
      stringStorage[words] = string;
      string = (char*)malloc(sizeof(char));

      if (string == NULL) {
	fprintf(stderr, "Error allocating memory.");
	exit(1);
      }

      words++;
      stringStorage = (char**)realloc(stringStorage, (words + 1)*sizeof(char*));

      if (stringStorage == NULL) {
	free(string);
	fprintf(stderr, "Error allocating memory.");
	exit(1);
      }

      numberOfChars = 0;
    }
    else {
      string[numberOfChars - 1] = input;
    }

  }

  qsort(stringStorage, words, sizeof(char*), compare);

  int k;

  for (k = 0; k < words; k++){
    int j = 0;
    while (1){
      if (stringStorage[k][j] == '\0' || stringStorage[k][j] == ' ') {
	break;
      }
      putchar(stringStorage[k][j]);
      j++;
    }
    putchar(stringStorage[k][j]);
  }

  for (k = 0; k < words; k++){
    free(stringStorage[k]);
  }
  free(stringStorage);

  exit(0);
}
