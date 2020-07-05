// challenge 2 includes Challenge 1
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
# define magicBytes 7 // starting magic bits + 1 for null character with using fgets
# define nameLen 17 // file name len
# define maxBlockSize 65536 // largest possible block size

// path to KBD file provided the command line arguments
// second arg is a check for challenge 3
unsigned char *readKDB(const char *file, int c3check){
  char filename[nameLen], magic[6];
  unsigned char *data[maxBlockSize];
  uint32_t entry_list, block_list, block_data, block_check;
  uint16_t block_size;
  int entryNum = 0;

  FILE *fp = fopen(file, "rb"); // open as a binary file
  if (fp == NULL){
    printf("Error in opening file.");
    return 1;
  }


  fgets(magic, magicBytes, fp);
  fread(&entry_list, sizeof(uint32_t), 1, fp); // reads the pointer to the entry list
  if (entry_list == NULL) {
    printf("Error in Finding the Entry List.");
    return 1;
  }

  fseek(fp, entry_list, SEEK_SET);

  while(1){ // reads at most 127 entries
    fseek(fp, entry_list, SEEK_SET);
    fseek(fp, entryNum * EntryLength, SEEK_CUR); // sets fp to the next entry in the entry_list

    fgets(filename, nameLen, fp);
    if (filename[0] == 0xffffffff) break;

    printf("%s : ", filename);

    fread(&block_list, sizeof(uint32_t), 1, fp);
    if (block_list == NULL){
      printf("Error reading block list\n");
      return 1;
    }

    while(1){ // traverse the block list of this particular entry
      fseek(fp, block_list, SEEK_SET);
      fread(&block_check, sizeof(uint32_t), 1, fp);
      if (block_check == 0xffffffff) {
        printf("No Blocks Found.\n");
        return 1;
      }
      fseek(fp, block_list, SEEK_SET);

      fread(&block_size, sizeof(uint16_t), 1, fp);
      fread(&block_data, sizeof(uint32_t), 1, fp);
      if (block_data == NULL){
        printf("Block reading error\n");
        return 1;
      }
      fread(&block_check, sizeof(uint32_t), 1, fp);

      // read the block's data
      fseek(fp, block_data, SEEK_SET);
      fread(&data, block_size, 1, fp);
      unsigned char *result = Crypt(data, block_size, initialValueKDB);
      printf("%s", result);
      if (c3check == 1) {
        printf("\n");
        return result;
      }

      memset(data, 0, sizeof(data));
      if (block_check == 0xffffffff) break;
    }

    printf(": End of Entry...\n");
    entryNum++;
  }
  fclose(fp);
  return 0;
}

// used for testing challenge 2 with store.kdb as input
 int main(int argc, const char* argv[]){
  readKDB(argv[1], 0);
  return 0;
}
