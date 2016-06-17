#include <string.h>
#include <stdio.h>
#include "data/instruction.h"
#include "disassembly.h"
#include "arch.h"

void disassemble_instruction(struct Instruction* i, char* buff) {
  switch(i->opcode) {

    // nop
    case 0x00:
      strcpy(buff, "nop");
      break;

    // mov Rd, Rs
    case 0x01:
      sprintf(buff, "mov %s, %s", REG_NAMES[i->dest], REG_NAMES[i->src]);
      break;

    // ldi Rd, I
    case 0x02:
      sprintf(buff, "ldi %s, 0x%x", REG_NAMES[i->dest], i->immediate);
      break;

    // ldm Rd
    case 0x03:
      sprintf(buff, "ldm %s", REG_NAMES[i->dest]);
      break;

    // lpx Rd
    case 0x04:
      sprintf(buff, "lpx %s", REG_NAMES[i->dest]);
      break;

    // spx Rs
    case 0x05:
      sprintf(buff, "spx %s", REG_NAMES[i->src]);
      break;

    // str Rs
    case 0x06:
      sprintf(buff, "str %s", REG_NAMES[i->src]);
      break;

    // push Rs
    case 0x07:
      sprintf(buff, "push %s", REG_NAMES[i->src]);
      break;

    // pop Rd
    case 0x08:
      sprintf(buff, "pop %s", REG_NAMES[i->dest]);
      break;

    // add Rd, Rs
    case 0x09:
      sprintf(buff, "add %s, %s", REG_NAMES[i->dest], REG_NAMES[i->src]);
      break;

    // addi Rd, I
    case 0x0a:
      sprintf(buff, "addi %s, 0x%x", REG_NAMES[i->dest], i->immediate);
      break;

    // sub Rd, Rs
    case 0x0b:
      sprintf(buff, "sub %s, %s", REG_NAMES[i->dest], REG_NAMES[i->src]);
      break;

    // subi Rd, I
    case 0x0c:
      sprintf(buff, "subi %s, 0x%x", REG_NAMES[i->dest], i->immediate);
      break;

    // cmp Ra, Rb
    case 0x0d:
      sprintf(buff, "cmp %s, %s", REG_NAMES[i->dest], REG_NAMES[i->src]);
      break;

    // and Rd, Rs
    case 0x0e:
      sprintf(buff, "and %s, %s", REG_NAMES[i->dest], REG_NAMES[i->src]);
      break;

    // or Rd, Rs
    case 0x0f:
      sprintf(buff, "or %s, %s", REG_NAMES[i->dest], REG_NAMES[i->src]);
      break;

    // xor Rd, Rs
    case 0x10:
      sprintf(buff, "xor %s, %s", REG_NAMES[i->dest], REG_NAMES[i->src]);
      break;

    // shl Rd
    case 0x11:
      sprintf(buff, "shl %s", REG_NAMES[i->dest]);
      break;

    // shr Rd
    case 0x12:
      sprintf(buff, "shr %s", REG_NAMES[i->dest]);
      break;

    // jmp Ra
    case 0x13:
      sprintf(buff, "jmp %s", REG_NAMES[i->dest]);
      break;

    // je Ra
    case 0x14:
      sprintf(buff, "je %s", REG_NAMES[i->dest]);
      break;

    // jne Ra
    case 0x15:
      sprintf(buff, "jne %s", REG_NAMES[i->dest]);
      break;

    // jg Ra
    case 0x16:
      sprintf(buff, "jg %s", REG_NAMES[i->dest]);
      break;

    // jge Ra
    case 0x17:
      sprintf(buff, "jge %s", REG_NAMES[i->dest]);
      break;

    // jl Ra
    case 0x18:
      sprintf(buff, "jl %s", REG_NAMES[i->dest]);
      break;

    // jle Ra
    case 0x19:
      sprintf(buff, "jle %s", REG_NAMES[i->dest]);
      break;

    // jz Ra
    case 0x1A:
      sprintf(buff, "jz %s", REG_NAMES[i->dest]);
      break;

    // jnz Ra
    case 0x1B:
      sprintf(buff, "jnz %s", REG_NAMES[i->dest]);
      break;

    // call Ra
    case 0x1C:
      sprintf(buff, "call %s", REG_NAMES[i->dest]);
      break;

    // calli I
    case 0x1D:
      sprintf(buff, "calli 0x%x", i->immediate);
      break;

    // ret
    case 0x1E:
      sprintf(buff, "ret");
      break;

    // hlt
    case 0xFF:
      sprintf(buff, "hlt");
      break;

    default:
      sprintf(
        buff,
        "UNKNOWN [O:0x%x D:0x%x S:0x%x I:0x%x]\n",
        i->opcode,
        i->dest,
        i->src,
        i->immediate
      );
  }
}
