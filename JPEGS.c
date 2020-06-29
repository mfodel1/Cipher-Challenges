// challenge 3
#include <stdio.h>
#include <string.h>
#include "KDB-Files.c"
#include <stdint.h>
#include <stdlib.h>


int main(int argc, const char *argv[]){
  // TO START:
    char jpgBuf[3], endCheck[2];
    unsigned char standard[3] = {0xFF, 0xD8, 0xFF};
    unsigned char jpgEnd[2] = {0xFF, 0xD9};
    // call readKdb to get magic bytes from given file.
    unsigned char *readResult = readKDB(argv[1], 1);
    long offset;
    int fileSize = 3; // initialized with 3 standard bytes.
    // read 3 bytes at a time from input.bin until the magic starting bytes are found.
    // find every JPEG file that starts with the custom magic bytes found in the KDB file
    FILE *fp = fopen(argv[2], "rb+");
    while(fp != EOF){
      fread(&jpgBuf, 3, 1, fp);
      if (jpgBuf == readResult){ // magic starting bits are found.
        // save the offset where the byte pattern was found, the size of the file
        offset = ftell(fp); // final offset will be offset - 3 since fread advances fp
        // replace the custom bits with the standard bits.
        fwrite(standard, 1, sizeof(standard), fp);
        // count the number of bytes in the file and write each word from input into a new output file.
        fread(&endCheck, 2, 1, fp);
        while (endCheck != jpgEnd) {
            size += fwrite();
          }
        }

      }
    }
    fclose(fp);

  // TO FINISH:
    // figure out the MD5 hash and what else to output.
    return 0;
}
