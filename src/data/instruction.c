#include "data/instruction.h"

void decode_instruction(uint32_t* data, struct Instruction* instruction) {
  instruction->opcode = *data & 0xff;
  instruction->dest = (*data >> 8) & 0xf;
  instruction->src = (*data >> 12) & 0xf;
  instruction->immediate = *data >> 16;
}

void encode_instruction(struct Instruction* instruction, uint32_t* data) {
  *data =
    (instruction->opcode << 16) |
    (instruction->dest << 12) |
    (instruction->src << 8) |
    instruction->opcode;
}
