#ifndef _DISASSEMBLY_H
#define _DISASSEMBLY_H

struct Instruction;

void disassemble_instruction(struct Instruction* instruction, char* buff);

#endif
