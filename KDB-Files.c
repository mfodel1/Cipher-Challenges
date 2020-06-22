// challenge 2
#include <stdio.h>
#include <string.h>
#include "LFSR.c"
#include <stdint.h>
#include <stdlib.h>

# define EntryLength 20 // bytes in entry
# define BlockLength 10 // bytes in block
# define maxBlocks 255
# define maxEntries 127
# define initialValueKDB 0x4F574154 // given intial value of LFSR for KDB files
# define magicBytes 7 // starting magic bits + 1 for null character
# define nameLen 17 // file name len

// path to KBD file provided the command line arguments
int main(int argc, const char *argv[]){
  char filename[nameLen], magic[6], *data, fileData[maxEntries][maxBlocks + 1], buffer[4]; // used for reading in the block_size and block_data
  uint32_t entry_list, block_list, block_data, block_check;
  uint16_t block_size;
  int entryNum = 0, blockNum = 0, totBlock = 0;

  FILE *fp = fopen("store.kdb", "r");
  if (fp == NULL){
    printf("Error in opening file.");
    return 1;
  }

  fgets(magic, magicBytes, fp);

  fread(&entry_list, sizeof(uint32_t), 1, fp); // reads the pointer to the entry list
  if (entry_list == NULL) {
    printf("No Entries in the entry list");
    return 1;
  }

  fseek(fp, entry_list, SEEK_SET); // moves fp to the start of the entry list

  while(1){ // reads at most 127 entries
    fgets(filename, nameLen, fp);
    if (filename == 0xffff) break;
    fileData[entryNum][0] = filename;

    fread(&block_list, sizeof(uint32_t), 1, fp);
    if (block_list == NULL){
      printf("Error reading block list\n");
      return 1;
    }

    while(1){ // traverse the block list of this particular entry
      fseek(fp, block_list, SEEK_SET);
      fseek(fp, blockNum * BlockLength, SEEK_CUR); // sets fp to the next block

      fread(&block_size, sizeof(uint16_t), 1, fp);
      fread(&block_data, sizeof(uint32_t), 1, fp);
      if (block_data == NULL){
        printf("Block reading error\n");
        return 1;
      }
      fread(&block_check, sizeof(uint32_t), 1, fp);

      // read the block's data
      fseek(fp, block_data, SEEK_SET);
      // NEEDS TO BE FIXED......
      // seg faults here
      data = (char *) malloc(block_size + 1);
      fgets(data, block_size + 1, fp);
      fileData[entryNum][blockNum + 1] = data; // save each block in a 2d array indexed by entry
      blockNum++;
      totBlock++;
      if (block_check == 0xffffffff) break;
    }

    entryNum++;
    fseek(fp, entry_list, SEEK_SET);
    fseek(fp, entryNum * EntryLength, SEEK_CUR); // sets fp to the next entry in the entry_list
  }

  fclose(fp);
  // for all the entries in the file, sent the data to LFSR and print the result + the filename
  int i, j;
  for (i = 0; i < entryNum; i++){
    for (j = 0; j < totBlock; j++){
      printf("%s\n", fileData[i][j]);
    }
  }
  return 0;
}
