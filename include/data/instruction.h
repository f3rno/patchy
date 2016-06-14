#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

#include <stdint.h>

struct Instruction {
  uint8_t opcode;
  uint8_t dest;
  uint8_t src;
  uint16_t immediate;
};

void decode_instruction(uint32_t* data, struct Instruction* instruction);
void encode_instruction(struct Instruction* instruction, uint32_t* data);

#endif
