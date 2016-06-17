#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "data/instruction.h"
#include "disassembly.h"

/**
 * TODO: Turn this into lib functionality to be used in both a seperate disass
 * binary, and with a flag in the emulator itself.
 */
int main(int argc, char *argv[]) {

  // TODO: Use the GNU C args parsing lib
  if(argc == 1) {
    printf("Usage todo, RTFM\n");
    return 1;
  } else if(argc > 2) {
    printf("Only the first argument is used!\n"); // Bah
  }

  uint32_t* in_word = malloc(sizeof(uint32_t));
  FILE* fp = fopen(argv[1], "rb");

  fseek(fp, 0, SEEK_END);
  int num_bytes = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  printf("Disassembling %s (%i bytes):\n\n", argv[1], num_bytes);

  unsigned char buff[1024];
  unsigned int address = 0;

  while(fread(in_word, sizeof(uint32_t), 1, fp) != 0) {
    struct Instruction* instruction = malloc(sizeof(struct Instruction));

    decode_instruction(in_word, instruction);
    disassemble_instruction(instruction, (char *)buff);

    printf("0x%x: %s\n", address, buff);
    address++;
  }

  int res = fclose(fp);

  if(res != 0) {
    printf("Error closing %s", argv[1]);
  }

  free(in_word);

  return 0;
}
