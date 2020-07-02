// challenge 3
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "KDB-Files.c"
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>


int main(int argc, const char *argv[]){
    unsigned char jpgBuf[4], endCheck[2];
    unsigned char magicStart[4];
    unsigned char standard[4] = {0xFF, 0xD8, 0xFF, 0xe0};
    unsigned char jpegEnd[2] = {0xFF, 0xD9};
    long offset;
    int fileSize = 4;

    // call readKdb to get magic bytes from given file.
    unsigned char *readResult = readKDB(argv[1], 1);
    int i;
    for (i = 0; i < 3; i++) {
      magicStart[i] = *(readResult + i);
    }
    magicStart[3] = 0xe0; // standard jpeg start
    FILE *fp = fopen(argv[2], "rb");

    char *dirName = strcat(argv[2], "_Repaired");
    mkdir(dirName);
    chdir(dirName);

    int num = fread(&jpgBuf, 1, sizeof(jpgBuf), fp);
    while(num == sizeof(jpgBuf)){
      if (memcmp(magicStart, jpgBuf, 4) == 0){
        // save the offset where the byte pattern was found
        offset = ftell(fp);
        offset -= 4;

        int num1 = fread(&endCheck, 1, sizeof(endCheck), fp);
        while (memcmp(endCheck, jpegEnd, 2) != 0 || num1 != sizeof(endCheck)){ // find the jpeg's size
            fileSize += 2; // 2 bytes read at a time.
            num1 = fread(&endCheck, 1, sizeof(endCheck), fp);
        }

        fseek(fp, offset, SEEK_SET); // sets fp to the start of the file
        unsigned char temp[fileSize]; /// problem making the array
        int num2 = fread(&temp, 1, fileSize, fp); // reads the file in its entirety from input file
        int i;
        for (i = 0; i < 3; i++){ // custom magic bytes are replaced
          temp[i] = standard[i];
        }

        char fileName[20];
        sprintf(fileName, "%x.jpeg", offset);
        FILE *fp1 = fopen(fileName, "w");
        fwrite(temp, sizeof(temp), 1, fp1);
        //memset(inputFile, 0, sizeof(inputFile));
        printf("File finished writing\n");
        fclose(fp1);
        // printf the file offset, fileSize, and the file's MD5 hash, and the filePath
        // MD5 hash.
        }
      num = fread(&jpgBuf, 1, sizeof(jpgBuf), fp);
      }
    fclose(fp);
    return 0;
}
