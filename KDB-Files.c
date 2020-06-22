// challenge 2
#include <stdio.h>
#include <string.h>
#include "LFSR.c"
#include <stdint.h>
#include <stdlib.h>

# define EntryLength 6 // bytes in entry
# define BlockLength 6 // bytes in block
# define maxBlocks 255
# define maxEntries 127
# define initialValueKDB 0x4F574154 // given intial value of LFSR for KDB files
# define magicBits 7 // starting magic bits + 1 for null character
# define nameLen 16 // file name len

// path to KBD file provided the command line arguments
int main(int argc, const char *argv[]){
  char filename[nameLen], magic[6], data, fileData[maxEntries][maxBlocks + 1];
  uint32_t entry_list, block_list, block_data;
  int entryNum = 0, blockNum = 0, totBlock = 0;
  uint32_t blockCheck, entryCheck;
  uint16_t block_size;

  FILE *fp = fopen("store.kdb", "r");
  if (fp == NULL){
    printf("Error in opening file.");
    return 1;
  }

  fgets(magic, magicBits, fp);

  fread(&entry_list, sizeof(uint32_t), 1, fp); // reads the pointer to the entry list
  if (entry_list == NULL) {
    printf("No Entries in the entry list");
    return 1;
  }

  fseek(fp, entry_list, SEEK_SET); // moves fp to the start of the entry list

  while(1){ // reads at most 127 entries
    fgets(filename, nameLen, fp);
    if (filename == 0xFF) break;
    fileData[entryNum][0] = filename;

    fread(&block_list, sizeof(uint32_t), 1, fp);
    if (block_list == NULL){
      printf("Error reading block list\n");
      return 1;
    }

    fseek(fp, block_list, SEEK_SET); // moves fp to the start of block_list

    while(1){ // traverse the block list of this particular entry
      fread(&block_size, sizeof(uint16_t), 1, fp);
      if (block_size == 0xFF) break;
      fread(&block_data, sizeof(uint32_t), 1, fp);
      if (block_data == NULL){
        printf("Block reading error\n");
        return 1;
      }

      // read the block's data
      fseek(fp, block_data, SEEK_SET);
      // allocate space to read in the variable length of data
      data = (char) malloc(block_size);
      fgets(data, block_size, fp);
      fileData[entryNum][blockNum + 1] = data; // save each block in a 2d array indexed by entry
      blockNum++;
      totBlock++;

      fseek(fp, block_list, SEEK_SET);
      fseek(fp, blockNum * BlockLength, SEEK_CUR); // sets fp to the next block
    }

    entryNum++;
    fseek(fp, entry_list, SEEK_SET);
    fseek(fp, entryNum * EntryLength, SEEK_CUR); // sets fp to the next entry in the entry_list
  }

  fclose(fp);
  free(data);
  // for all the entries in the file, sent the data to LFSR and print the result + the filename
  int i, j;
  for (i = 0; i < entryNum; i++){
    for (j = 0; j < totBlock; j++){
      printf("%s\n", fileData[i][j]);
    }
  }
  return 0;
}
