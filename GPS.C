#include <stdio.h>
#include <string.h>

// Function to encrypt a character
char *encrypt(char inputChar) {
 static char result[3];
  if (inputChar == '\t') {
    result[0] = 'T';
    result[1] = 'T';  
    result[2] = '\0';

    return result;

  } else if (inputChar == '\n') {
    result[0] = inputChar;
    result[1] = '\0';
    return result;

  } else {
    int outChar = (int)inputChar - 16;
    if (outChar < 32) {
      outChar = (outChar - 32) + 144;
    }
    snprintf(result, sizeof(result), "%02X", outChar);
  }

  return result;
}


char decrypt(char char1, char char2) {
  if (char1 == 'T' && char2 == 'T') {
    return '\t';
  }
  else {
    int int1, int2;
    if (char1 >= 'A' && char1 <= 'F') {
      int1 = (char1 - 'A' + 10) * 16;
    } else {
      int1 = (char1 - '0') * 16;
    }

    if (char2 >= 'A' && char2 <= 'F') {
      int2 = char2 - 'A' + 10;
    } else {
      int2 = char2 - '0';
    }

    int outChar = int1 + int2;
    outChar += 16;

    if (outChar > 127) {
      outChar = (outChar - 144) + 32;
    }

    return (char)outChar;
  }
}

int main(int argcount, char *argv[]) {
  if (argcount != 3 && argcount != 4) {
    printf("Usage: cryptoMagic [-E|-D] input_filename [output_filename]\n");
    return 1;
  }

  int decrypt1=0;
  char *inputFilename = argv[2];
  char *outputFilename;
  if(argcount == 4 && strcmp(argv[1], "-D") == 0) {
        decrypt1 = 1;
        inputFilename = argv[2];
        outputFilename = argv[3];
    } else if(argcount == 4 ){
        inputFilename = argv[2];
        outputFilename = argv[3];
    }
    else
    {
      inputFilename = argv[1];
      outputFilename = argv[2];
    }

  FILE *inputfile = fopen(inputFilename, "r");
  FILE *outputfile = fopen(outputFilename, "w");

  if (inputfile == NULL || outputfile == NULL) {
    printf("Error.\n");
    return 1;
  }

  char inputChar;
  char char1, char2;
  int pairFlag = 0;

  while ((inputChar = fgetc(inputfile)) != EOF) {
    if (inputChar == '\n')
    {
      if (decrypt1 == 1) {
        pairFlag = 0;
      }
      fputc(inputChar, outputfile);
    }
    else{


    if (decrypt1 == 0) {
      const char *encryptedChar = encrypt(inputChar);
      fputs(encryptedChar, outputfile);
    }
      else
    {

      if (pairFlag == 0)
       {
        char1 = inputChar;
        pairFlag = 1;
       }

      else {
        char2 = inputChar;
        char decryptedChar = decrypt(char1, char2);
        fputc(decryptedChar, outputfile);
        pairFlag = 0;
      }
    }
  }
}  

  fclose(inputfile);
  fclose(outputfile);

  return 0;
  }
