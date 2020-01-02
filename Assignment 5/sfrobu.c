#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>

bool ignoreCase = false;

int frobcmp(const char* a, const char* b) {
  while (*a != ' ' || *b != ' ') {

    char realA = (*a)^0x2a;
    char realB = (*b)^0x2a;

    if (ignoreCase == true) {
      realA = toupper((unsigned)realA);
      realB = toupper((unsigned)realB);
    }
    
    if (realA > realB || *b == ' ') {
      return 1;
    }
    else if (realB > realA || *a == ' ') {
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

int main(int argc, const char* argv[]) {
  if (argc > 2) {
    write(2, "Incorrect number of arguments.", 30);
    exit(1);
  }

  if (argc == 2) {
    if (strcmp(argv[1], "-f") != 0){
      write(2, "Incorrect argument input.", 25);
      exit(1);
    }
    ignoreCase = true;
  }

  struct stat fileS;
  if (fstat(0, &fileS) < 0) {
    write(2, "Unable to read file.", 20);
    exit(1);
  }
  
  char** stringStorage = NULL;
  int words = 0;
  int numberOfChars = 0;

  stringStorage = malloc(sizeof(char*)*(words + 1));

  if (stringStorage == NULL) {
    write(2, "Can't allocate memory.", 22);
    exit(1);	
  }
  
  if (S_ISREG(fileS.st_mode)) {
    int size = fileS.st_size;
    
    char* buffer = malloc(sizeof(char)*size);
    
    if (buffer == NULL) {
      write(2, "Can't allocate memory.", 22);
      exit(1);
    }

	
    if (read(0, buffer, size) < 0) {
      write(2, "Unable to read file.", 20);
      exit(1);
    }

    int k = 0;

    char* string = NULL;

    while (k < size) {
      
      numberOfChars++;

      string = realloc(string, sizeof(char)*numberOfChars); 
      
      if (buffer[k] == ' ') {
	string[numberOfChars - 1] = ' ';
	stringStorage[words] = string;
	numberOfChars = 0;
	words++; 
	stringStorage = realloc(stringStorage, sizeof(char*)*(words+1));
	if (stringStorage == NULL) {
	  write(2, "Can't allocate memory.", 22);
	  exit(1); 	
	} 
	string = NULL;
      }

      else {
	string[numberOfChars - 1] = buffer[k];
     }
      
      k++;
    }

    if (buffer[size - 1] != ' ') {
      numberOfChars++;
      string[numberOfChars - 1] = ' ';
      stringStorage[words] = string;
      words++;
      numberOfChars = 0;
    }
    free(buffer);
    
    if (fstat(0, &fileS) < 0) {
      write(2, "Unable to read file.", 20);
      exit(1);
    }
    char* Movebuffer;

    if (fileS.st_size > size) {
      lseek(0, 0, SEEK_CUR);
      size = fileS.st_size - size;
      string = NULL;
      stringStorage = realloc(stringStorage, sizeof(char*)*(words+1));
      Movebuffer = malloc(sizeof(char)*size);

      if (Movebuffer == NULL) {
	write(2, "Can't allocate memory.", 22);
	exit(1);
      }
    

      if (read(0, Movebuffer, size) < 0) {
      	write(2, "Unable to read file.", 20);
      	exit(1);
    
      }
      k = 0;

      while (k < size) {
      	numberOfChars++;

      	string = realloc(string, sizeof(char)*numberOfChars);

      	if (Movebuffer[k] == ' ') {
		string[numberOfChars - 1] = ' ';
		stringStorage[words] = string;
		numberOfChars = 0;
		words++;
		stringStorage = realloc(stringStorage, sizeof(char*)*(words+1));
	
	if (stringStorage == NULL) {
	  write(2, "Can't allocate memory.", 22);
	  exit(1);
	}
      
	string = NULL;
      }

      else {
	string[numberOfChars - 1] = buffer[k];
      }
      k++;
    }

    if (Movebuffer[size - 1] != ' ') {
      numberOfChars++;
      string[numberOfChars - 1] = ' ';
      
      stringStorage[words] = string;
      words++;
      numberOfChars = 0;
    }
  
  }
  }

  else {
    char buf;
    
    char* string = NULL;
    
    char lastChar;

    while(read(0,&buf,1)>0) {
      numberOfChars++;
      lastChar = buf;
      string = realloc(string, sizeof(char)*numberOfChars);
      
      
      if (string == NULL) {
	write(2, "Can't allocate memory.", 22);
	exit(1);
      }

      if (buf == ' ') {
	string[numberOfChars - 1] = ' ';
	stringStorage[words] = string;
	words++;
	string = NULL;
	stringStorage = realloc(stringStorage, (words +1)*sizeof(char*)); 
	numberOfChars = 0;
      }
      
      else {
	string[numberOfChars - 1] = buf;
      }
    }
    if (lastChar != ' ') {
      numberOfChars++;
      string[numberOfChars - 1] = ' ';
      stringStorage[words] = string;
      words++;
      numberOfChars = 0;
    }
   
  }

  qsort(stringStorage, words, sizeof(char*), compare);

  int i;

  for (i = 0; i < words; i++) {
    int j = 0;
    while(1) {
      if (stringStorage[i][j] == '\n' || stringStorage[i][j] == ' ') {
	
	 char output[1];
	 output[0] = stringStorage[i][j];
	 if (write(1,output, 1) < 0) {
	   write(2, "Error in output.", 16);
	   exit(1);
	 }
       
	
	break;
      }
      char output[1];
      output[0] = stringStorage[i][j];
      if (write(1,output,1) < 0) {
	write(2, "Error in output.", 16);
	exit(1);
      }
      j++;
    }
  }
  
  for (i = 0; i < words; i++) {
    free(stringStorage[i]);
  }
  free(stringStorage);
  exit(0);

}
