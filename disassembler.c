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

  uint32_t* in_word = malloc(sizeof(uint32_t));
  FILE* fp = fopen("pong-test.bin", "rb");

  unsigned char buff[1024];
  unsigned int address = 0;

  while(fread(in_word, sizeof(uint32_t), 1, fp) != 0) {
    struct Instruction* instruction = malloc(sizeof(struct Instruction));

    decode_instruction(in_word, instruction);
    disassemble_instruction(instruction, buff);

    printf("0x%x: %s\n", address, buff);
    address++;
  }

  int res = fclose(fp);

  if(res != 0) {
    printf("Error closing pong-test.bin");
  }

  free(in_word);

  return 0;
}
