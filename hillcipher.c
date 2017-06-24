#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char *ReadFile(char *fileName);
void PadPlainText(char *plainText, int matrixSize);
int CharIndex(char letter);
void RemoveSpaces(char *str);
void OutputKeyFile(int size, int matrix[size][size]);
void OutputPlainText(char *text);
void OutputCipherText(char *text, int size);
int *TextToIntegers(char *text);
char *PlainTextToCipherText(char *plainText, int size, int keyMatrix[size][size]);
char *EncodeCharacters(int size, int keyMatrix[size][size], int *dataMatrix);

const char alphabet[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
int fileSize;

int main(int argc, char *argv[] ){
  FILE *keyFile;
  int matrixSize;

  //Add some blank lines for clarity
  printf("\n\n");

  //Read key file into memory
  keyFile = fopen(argv[1], "r");

  //Set matrix size
  fscanf(keyFile,"%d", &matrixSize);

  //Create matrix array
  int keyMatrix[matrixSize][matrixSize];
  int i,j;
  for(i=0; i < matrixSize; i++){
    for(j=0; j< matrixSize; j++){
      fscanf(keyFile, "%d", &keyMatrix[i][j]);
    }
  }
  //Output Key File
  OutputKeyFile(matrixSize, keyMatrix);

  //Read text file into memory
  char *plainText = ReadFile(argv[2]);
  //Apply padding if necessary
  PadPlainText(plainText, matrixSize);



  //Output Plain Text
  OutputPlainText(plainText);

  //Convert Plain Text To Cipher Text
  char *cipherText = PlainTextToCipherText(plainText, matrixSize, keyMatrix);


  //Output Cipher Text
  OutputCipherText(cipherText, strlen(plainText));
  printf("\n");


  return 0;
}

char *PlainTextToCipherText(char *plainText, int matrixSize, int keyMatrix[matrixSize][matrixSize]){
  int size = strlen(plainText);

  int *plainTextNumeric = TextToIntegers(plainText);

  int counter = 0;
  char *cipherText = NULL;
  cipherText = malloc((size + 1) * sizeof(*cipherText));

  while (counter < strlen(plainText)){
    //Break text file into chunks of size of the matrix
    int chunk[matrixSize];
    for(int j = 0; j < matrixSize; j++){
      chunk[j] = plainTextNumeric[j + counter];
    }
    char *cipherChunk = NULL;
    cipherChunk = malloc((matrixSize + 1) * sizeof(*cipherChunk));

    //Apply cipher to each chunk
    cipherChunk = EncodeCharacters(matrixSize, keyMatrix, chunk);

    //Write out cipher text to final array
    for(int j=0; j < matrixSize; j++){
      cipherText[j + counter] = cipherChunk[j];
    }
    counter += matrixSize;
  }
  return cipherText;
}


char *ReadFile(char *fileName){
  char *buffer = NULL;
  size_t size = 0;
  FILE *fp = fopen(fileName, "r");
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);
  buffer = malloc((size + 1) * sizeof(*buffer));
  fread(buffer, size, 1, fp);
  buffer[size] = '\0';
  fileSize = size;
  RemoveSpaces(buffer);
  return buffer;
}

void PadPlainText(char *plainText, int matrixSize){
  int counter = 0;
  while(strlen(plainText) % matrixSize > 0 && counter++ < matrixSize*2){
      plainText[strlen(plainText)] = 'x';
  }
  plainText[strlen(plainText)] = '\0';
}

int CharIndex(char letter){
  int i;
  for(i = 0; i < 26; i++){
    if(tolower(letter) == alphabet[i]){
      return i;
    }
  }
  return -1;
}

void RemoveSpaces(char *str){
    int count = 0;
    for (int i = 0; str[i]; i++){
      int index = CharIndex(str[i]);
      if(index >= 0){
        str[count++] = alphabet[index];
      }
    }
    str[count] = '\0';
    for(int j = count; j < fileSize; j ++){
      str[j] = '\0';
    }

}

void OutputKeyFile(int size, int matrix[size][size]){
  printf("Key Matrix:\n\n");
  int i,j;
  for(i =0; i < size; i++){
    for(j=0; j < size; j++){
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n\n");
}

void OutputPlainText(char *text){
  printf("Plain Text:\n\n%s\n\n\n", text);
}

int *TextToIntegers(char *text){
  int charLength = strlen(text);
  int *textNumeric = NULL;
  textNumeric = malloc((charLength + 1) * sizeof(*textNumeric));
  for(int i=0; i < charLength; i++){
    textNumeric[i] = CharIndex(text[i]);
  }
  return textNumeric;
}

char *EncodeCharacters(int size, int keyMatrix[size][size], int *dataMatrix){
  char *results = NULL;
  results = malloc((size + 1) * sizeof(*results));
  for(int i = 0; i < size; i++){
    int data = 0;
    for(int j = 0; j < size; j++){
      data += keyMatrix[i][j] * dataMatrix[j];
    }
    results[i] = alphabet[data % 26];
  }
  return results;
}

void OutputCipherText(char *text, int size){
  printf("Cipher Text:\n\n");
  for(int i = 0; i < size; i++){
    printf("%c", text[i]);
    if(size % 80 == 0){
      printf("\n");
    }
  }
}
