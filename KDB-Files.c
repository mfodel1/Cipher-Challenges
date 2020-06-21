// challenge
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
# define nameLen 16 // file names are stored as null terminated strings

// path to KBD file provided the command line arguments
int main(int argc, const char *argv[]){
  char filename[nameLen];
  char *magic;
  char data;
  uint32_t entry_list, block_list, block_data;
  int entryNum = 0, blockNum = 0;
  uint32_t blockCheck, entryCheck;
  uint16_t block_size;
  char fileData[maxEntries][maxBlocks];
  char fileNameList[maxEntries];

  FILE *fp = fopen("store.kdb", "r");
  if (fp == NULL){
    printf("Error in opening file.");
    return 1;
  }

  fgets(magic, magicBits, fp);

  fread(entry_list, sizeof(uint32_t), 1, fp); // reads the pointer to the entry list
  if (entry_list == NULL) {
    printf("No Entries in the entry list");
    return 1;
  }

  fseek(fp, entry_list, SEEK_SET); // moves fp to the start of the entry list

  while(1){ // reads at most 127 entries
    fgets(filename, nameLen, fp);
    fileNameList[entryNum] = filename;

    fread(block_list, sizeof(uint32_t), 1, fp);
    if (block_list == NULL){
      printf("Error reading block list\n");
      return 1;
    }

    fseek(fp, block_list, SEEK_SET); // moves fp to the start of block_list
    blockNum = 0;

    while(1){ // traverse the block list of this particular entry
      fread(&block_size, sizeof(uint16_t), 1, fp);
      fseek(fp, sizeof(uint16_t), SEEK_CUR);
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
      fileData[entryNum][blockNum] = data; // save each block in a 2d array indexed by entry

      blockNum++;
      fseek(fp, block_list, SEEK_SET); // moves fp to the start of entries of block_list
      fseek(fp, blockNum * BlockLength, SEEK_CUR); // move fp to the next block in block_list
      fread(&blockCheck, sizeof(uint32_t), 1, fp); // used to check if the block list is done
      if (blockCheck == 0xFFFFFFFF) {
        break;
      }
    }

    entryNum++;
    fseek(fp, entry_list, SEEK_SET);
    fseek(fp, entryNum * EntryLength, SEEK_CUR); // sets fp to the next entry in the entry_list
    fread(&entryCheck, sizeof(uint32_t), 1, fp); // used to check if the entry list is done
    if (entryCheck == 0xFFFFFFFF) {
      break;
    }

  }

  fclose(fp);
  free(data);
  // for all the entries in the file, sent the data to LFSR and print the result + the filename
  return 0;
}
