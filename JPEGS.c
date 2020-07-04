// challenge 3
#include <stdio.h>
#include <string.h>
#include "KDB-Files.c"
#include <openssl/md5.h>

int main(int argc, const char *argv[]){
    unsigned char jpgBuf[4], temp[2];
    unsigned char magicStart[4];
    unsigned char standard[4] = {0xFF, 0xD8, 0xFF, 0xe0};
    unsigned char jpegEnd[2] = {0xFF, 0xD9};
    long offset, inputSize, check1;
    int fileSize = 0;
    int endCheck = 0;

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

    fseek(fp, 0, SEEK_END);
    inputSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    while(ftell(fp) < inputSize){
      fread(&jpgBuf, 1, sizeof(jpgBuf), fp);
      if (memcmp(magicStart, jpgBuf, 4) == 0){
        // save the offset where the byte pattern was found
        offset = ftell(fp);
        offset -= 4;

        char fileName[20];
        sprintf(fileName, "%x.jpeg", offset);
        FILE *fp1 = fopen(fileName, "wb");
        fwrite(standard, 1, sizeof(standard), fp1);

        fread(&temp, 1, sizeof(temp), fp);
        while (endCheck != 1){
          if (memcmp(temp, jpegEnd, 2) == 0){
            fwrite(temp, 1, 2, fp1);
            endCheck = 1;
          }
          else{
            fwrite(temp, 1, 2, fp1);
            fread(&temp, 1, sizeof(temp), fp);
          }
        }

        fileSize = ftell(fp) - offset;
        fclose(fp1);

        unsigned char data[fileSize - 1];
        fp1 = fopen(fileName, "rb");
        int num = fread(&data, 1, fileSize - 1, fp1);
        fclose(fp1);

        unsigned char hash[MD5_DIGEST_LENGTH];
        MD5(data, num, hash);

        int reset = ftell(fp) % 16;
        fseek(fp, fileSize + offset - reset, SEEK_SET);

        endCheck = 0;

        printf("File: %s\n", fileName);
        printf("Offset found: %x\n", offset);
        printf("File Size: %d\n", fileSize);
        printf("File Path : %s/%s\n", dirName, fileName);
        printf("MD5 Hash: ")
        int i;
        for(i = 0; i < MD5_DIGEST_LENGTH; i++){
          printf("%02x", hash[i]);
        }
        printf("\n");
        printf("End of File Data\n");
        printf("\n\n");
        }
      }
    fclose(fp);
    return 0;
}
