#ifndef _ARCH_H
#define _ARCH_H

extern const char* const REG_NAMES[];

enum PATCHY_FLAG {
  LT,
  GT,
  EQ,
  HLT,
  ZE
};

enum PATCHY_OPCODES {
  OP_NOOP =  0x00,

  // Register/memory access
  OP_MOV =   0x01,
  OP_LDI =   0x02,
  OP_LDM =   0x03,
  OP_LPX =   0x04,
  OP_SPX =   0x05,
  OP_STR =   0x06,

  // Stack
  OP_PUSH =  0x07,
  OP_POP =   0x08,

  // ALU
  OP_ADD =   0x09,
  OP_ADDI =  0x0a,
  OP_SUB =   0x0b,
  OP_SUBI =  0x0c,
  OP_CMP =   0x0d,
  OP_AND =   0x0e,
  OP_OR =    0x0f,
  OP_XOR =   0x10,
  OP_SHL =   0x11,
  OP_SHR =   0x12,

  // Branching
  OP_JMP =   0x13,
  OP_JE =    0x14,
  OP_JNE =   0x15,
  OP_JG =    0x16,
  OP_JGE =   0x17,
  OP_JL =    0x18,
  OP_JLE =   0x19,
  OP_JZ =    0x1a,
  OP_JNZ =   0x1b,

  // Subroutines
  OP_CALL =  0x1c,
  OP_CALLI = 0x1d,
  OP_RET =   0x1e,

  OP_HLT =   0xff
};

#endif
