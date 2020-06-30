// challenge 3
#include <stdio.h>
#include <string.h>
#include "KDB-Files.c"
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, const char *argv[]){
    unsigned char jpgBuf[3], endCheck[2];
    unsigned char magicStart[3];
    unsigned char standard[3] = {0xFF, 0xD8, 0xFF};
    unsigned char jpegEnd[2] = {0xFF, 0xD9};
    long offset;
    int fileSize;

    // call readKdb to get magic bytes from given file.
    unsigned char *readResult = readKDB(argv[1], 1);
    int i;
    for (i = 0; i < 3; i++) {
      magicStart[i] = *(readResult + i);
    }
    FILE *fp = fopen(argv[2], "r");

    char *dirName = strcat(argv[2], "_Repaired");
    mkdir(dirName);
    strcat(dirName, "/");

    int num = fread(&jpgBuf, 1, sizeof(jpgBuf), fp);
    while(num == sizeof(jpgBuf)){
      int ret = memcmp(magicStart, jpgBuf, 3);
      if (ret == 0){
        // save the offset where the byte pattern was found
        offset = ftell(fp);
        offset -= 3;
        char *fileName;
        sprintf(fileName, "%x.jpeg", offset);
        char *filePath = strcat(dirName, fileName);
        FILE *fp1 = fopen(filePath, "w");
        fileSize += fwrite(standard, 1, sizeof(standard), fp1);
        fread(&endCheck, 2, 1, fp);
        while (endCheck != jpegEnd) {
            fileSize += fwrite(endCheck, 1, 2, fp1);
            fread(&endCheck, 2, 1, fp);
          }
        fileSize += fwrite(endCheck, 1, 2, fp1);
        printf("File finished writing\n");
        // printf the file offset, fileSize, and the file's MD5 hash, and the filePath
        fclose(fp1);
        // MD5 hash.
        }
      num = fread(&jpgBuf, 1, sizeof(jpgBuf), fp);
      }
    fclose(fp);
    return 0;
}
