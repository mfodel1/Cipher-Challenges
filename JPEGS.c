// challenge 3
#include <stdio.h>
#include <string.h>
#include "KDB-Files.c"
#include <stdint.h>
#include <stdlib.h>


int main(int argc, const char *argv[]){
  // TO START:
    // call readKdb to get magic bytes.
    unsigned char *readResult = readKDB(argv[1]); // need to fix how the magic bytes are loaded into this function.

    // read whole input.bin file continually until the magic bytes are found.
    // find every JPEG file that starts with the custom magic bytes found in the KDB file
    // replace the custom bits with the standard bits.
    // fwrite();
  // TO FINISH:
    // figure out the MD5 hash and what to output.
    return 0;
}
