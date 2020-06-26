// challenge 3
#include <stdio.h>
#include <string.h>
#include "KDB-Files.c"
#include <stdint.h>
#include <stdlib.h>


int main(int argc, const char *argv[]){
  // TO START:
    char magicStart[3];
    unsigned char standard[3] = {0xFF, 0xD8, 0xFF};
    // call readKdb to get magic bytes.
    unsigned char *readResult = readKDB(argv[1], 1);

    // read 3 bytes at a time from input.bin until the magic starting bytes are found.
    // find every JPEG file that starts with the custom magic bytes found in the KDB file
    FILE *fp = fopen(argv[2], "rb");
    while(fp != EOF){
      fread(&magicStart, 3, 1, fp);
      if (magicStart == readResult){ // magic starting bits are found.
        // save the offset where the byte pattern was found, the size of the file.
        // replace the custom bits with the standard bits.
        fwrite(standard, 1, sizeof(standard), fp);
      }
    }
    fclose(fp);

  // TO FINISH:
    // figure out the MD5 hash and what else to output.
    return 0;
}
