#include <stdbool.h>
#include <stdio.h>

#include "arch.h"
#include "exec.h"
#include "core.h"
#include "data/memory.h"
#include "data/instruction.h"
#include "disassembly.h"

void patchy_exec(struct PatchyCore* core) {
  bool done = false;

  struct Instruction instruction;
  char i_buff[1024];

  while(!done && !core->halted) {
    decode_instruction(&core->rom->contents[core->reg_ip], &instruction);

    // TODO: Add debug flag
    // disassemble_instruction(&instruction, &i_buff);
    // printf("  %s\n", i_buff);

    switch(instruction.opcode) {
      case 0x01:
        patchy_exec_mov(core, &instruction);
        break;

      case 0x02:
        patchy_exec_ldi(core, &instruction);
        break;

      case 0x03:
        patchy_exec_ldm(core, &instruction);
        break;

      case 0x04:
        patchy_exec_lpx(core, &instruction);
        break;

      case 0x05:
        patchy_exec_spx(core, &instruction);
        break;

      case 0x06:
        patchy_exec_str(core, &instruction);
        break;

      case 0x07:
        patchy_exec_push(core, &instruction);
        break;

      case 0x08:
        patchy_exec_pop(core, &instruction);
        break;

      case 0x09:
        patchy_exec_add(core, &instruction);
        break;

      case 0x0a:
        patchy_exec_addi(core, &instruction);
        break;

      case 0x0b:
        patchy_exec_sub(core, &instruction);
        break;

      case 0x0c:
        patchy_exec_subi(core, &instruction);
        break;

      case 0x0d:
        patchy_exec_cmp(core, &instruction);
        break;

      case 0x0e:
        patchy_exec_and(core, &instruction);
        break;

      case 0x0f:
        patchy_exec_or(core, &instruction);
        break;

      case 0x10:
        patchy_exec_xor(core, &instruction);
        break;

      case 0x11:
        patchy_exec_shl(core, &instruction);
        break;

      case 0x12:
        patchy_exec_shr(core, &instruction);
        break;

      case 0x13:
        patchy_exec_jmp(core, &instruction);
        break;

      case 0x14:
        patchy_exec_je(core, &instruction);
        break;

      case 0x15:
        patchy_exec_jne(core, &instruction);
        break;

      case 0x16:
        patchy_exec_jg(core, &instruction);
        break;

      case 0x17:
        patchy_exec_jge(core, &instruction);
        break;

      case 0x18:
        patchy_exec_jl(core, &instruction);
        break;

      case 0x19:
        patchy_exec_jle(core, &instruction);
        break;

      case 0x1a:
        patchy_exec_jz(core, &instruction);
        break;

      case 0x1b:
        patchy_exec_jnz(core, &instruction);
        break;

      case 0x1c:
        patchy_exec_call(core, &instruction);
        break;

      case 0x1d:
        patchy_exec_calli(core, &instruction);
        break;

      case 0x1e:
        patchy_exec_ret(core);
        break;

      case 0xff:
        patchy_exec_hlt(core);
        break;

      // Noop
      case 0x00:
        break;

      default:
        printf("[0x%x]: Unknown op: 0x%x\n", core->reg_ip, instruction.opcode);
        break;
    }

    if(core->halted) {
      printf("Halted\n");
    } else {
      core->reg_ip++;

      if(core->reg_ip == 0) {
        done = true;

        printf("Reached end of ROM, preventing cyclical execution, halted\n");
      }
    }
  }
}

void jmp_to_reg_by_adr(struct PatchyCore* core, uint8_t addr) {
  core->reg_ip = *get_core_reg_by_adr(core, addr) - 1;
}

void patchy_exec_mov(struct PatchyCore* core, struct Instruction* i) {
  uint16_t* dest = get_core_reg_by_adr(core, i->dest);
  uint16_t* src = get_core_reg_by_adr(core, i->src);

  *dest = *src;
}

void patchy_exec_ldi(struct PatchyCore* core, struct Instruction* i) {
  uint16_t* dest = get_core_reg_by_adr(core, i->dest);

  *dest = i->immediate;
}

void patchy_exec_ldm(struct PatchyCore* core, struct Instruction* i) {
  uint16_t* dest = get_core_reg_by_adr(core, i->dest);

  *dest = core->ram->contents[core->reg_dp];
}

void patchy_exec_lpx(struct PatchyCore* core, struct Instruction* i) {
  // TODO
}

void patchy_exec_spx(struct PatchyCore* core, struct Instruction* i) {
  // TODO
}

void patchy_exec_str(struct PatchyCore* core, struct Instruction* i) {
  uint16_t* src = get_core_reg_by_adr(core, i->src);

  core->ram->contents[core->reg_dp] = *src;
}

void patchy_exec_push(struct PatchyCore* core, struct Instruction* i) {
  uint16_t* src = get_core_reg_by_adr(core, i->src);

  core->reg_sp--;
  core->ram->contents[core->reg_sp] = *src;
}

void patchy_exec_pop(struct PatchyCore* core, struct Instruction* i) {
  uint16_t* dest = get_core_reg_by_adr(core, i->dest);

  *dest = core->ram->contents[core->reg_sp];
  core->reg_sp++;
}

void patchy_exec_add(struct PatchyCore* core, struct Instruction* i) {
  uint16_t* dest = get_core_reg_by_adr(core, i->dest);
  uint16_t* src = get_core_reg_by_adr(core, i->src);

  *dest = *dest + *src;
  set_core_flag(core, ZE, *dest == 0);
}

void patchy_exec_addi(struct PatchyCore* core, struct Instruction* i) {
  uint16_t* dest = get_core_reg_by_adr(core, i->dest);

  *dest = *dest + i->immediate;
  set_core_flag(core, ZE, *dest == 0);
}

void patchy_exec_sub(struct PatchyCore* core, struct Instruction* i) {
  uint16_t* dest = get_core_reg_by_adr(core, i->dest);
  uint16_t* src = get_core_reg_by_adr(core, i->src);

  *dest = *dest - *src;
  set_core_flag(core, ZE, *dest == 0);
}

void patchy_exec_subi(struct PatchyCore* core, struct Instruction* i) {
  uint16_t* dest = get_core_reg_by_adr(core, i->dest);

  *dest = *dest - i->immediate;
  set_core_flag(core, ZE, *dest == 0);
}

void patchy_exec_cmp(struct PatchyCore* core, struct Instruction* i) {
  uint16_t* a = get_core_reg_by_adr(core, i->dest);
  uint16_t* b = get_core_reg_by_adr(core, i->src);

  set_core_flag(core, LT, *a < *b);
  set_core_flag(core, GT, *a > *b);
  set_core_flag(core, EQ, *a == *b);
}

void patchy_exec_and(struct PatchyCore* core, struct Instruction* i) {
  uint16_t* dest = get_core_reg_by_adr(core, i->dest);
  uint16_t* src = get_core_reg_by_adr(core, i->src);

  *dest = *dest & *src;
}

void patchy_exec_or(struct PatchyCore* core, struct Instruction* i) {
  uint16_t* dest = get_core_reg_by_adr(core, i->dest);
  uint16_t* src = get_core_reg_by_adr(core, i->src);

  *dest = *dest | *src;
}

void patchy_exec_xor(struct PatchyCore* core, struct Instruction* i) {
  uint16_t* dest = get_core_reg_by_adr(core, i->dest);
  uint16_t* src = get_core_reg_by_adr(core, i->src);

  *dest = *dest ^ *src;
}

void patchy_exec_shl(struct PatchyCore* core, struct Instruction* i) {
  uint16_t* dest = get_core_reg_by_adr(core, i->dest);

  *dest = *dest << 1;
}

void patchy_exec_shr(struct PatchyCore* core, struct Instruction* i) {
  uint16_t* dest = get_core_reg_by_adr(core, i->dest);

  *dest = *dest >> 1;
}

void patchy_exec_jmp(struct PatchyCore* core, struct Instruction* i) {
  jmp_to_reg_by_adr(core, i->dest);
}

void patchy_exec_je(struct PatchyCore* core, struct Instruction* i) {
  if(get_core_flag(core, EQ)) {
    jmp_to_reg_by_adr(core, i->dest);
  }
}

void patchy_exec_jne(struct PatchyCore* core, struct Instruction* i) {
  if(!get_core_flag(core, EQ)) {
    jmp_to_reg_by_adr(core, i->dest);
  }
}

void patchy_exec_jg(struct PatchyCore* core, struct Instruction* i) {
  if(get_core_flag(core, GT)) {
    jmp_to_reg_by_adr(core, i->dest);
  }
}

void patchy_exec_jge(struct PatchyCore* core, struct Instruction* i) {
  if(get_core_flag(core, GT) || get_core_flag(core, EQ)) {
    jmp_to_reg_by_adr(core, i->dest);
  }
}

void patchy_exec_jl(struct PatchyCore* core, struct Instruction* i) {
  if(get_core_flag(core, LT)) {
    jmp_to_reg_by_adr(core, i->dest);
  }
}

void patchy_exec_jle(struct PatchyCore* core, struct Instruction* i) {
  if(get_core_flag(core, LT) || get_core_flag(core, EQ)) {
    jmp_to_reg_by_adr(core, i->dest);
  }
}

void patchy_exec_jz(struct PatchyCore* core, struct Instruction* i) {
  if(get_core_flag(core, ZE)) {
    jmp_to_reg_by_adr(core, i->dest);
  }
}

void patchy_exec_jnz(struct PatchyCore* core, struct Instruction* i) {
  if(!get_core_flag(core, ZE)) {
    jmp_to_reg_by_adr(core, i->dest);
  }
}

void patchy_exec_call(struct PatchyCore* core, struct Instruction* i) {
  // Push IP + 1 onto stack
  core->reg_sp--;
  core->ram->contents[core->reg_sp] = core->reg_ip + 1;

  jmp_to_reg_by_adr(core, i->dest);
}

void patchy_exec_calli(struct PatchyCore* core, struct Instruction* i) {
  // Push IP + 1 onto stack
  core->reg_sp--;
  core->ram->contents[core->reg_sp] = core->reg_ip + 1;

  // IP is immediately incremented
  core->reg_ip = i->immediate - 1;
}

void patchy_exec_ret(struct PatchyCore* core) {
  uint16_t address = core->ram->contents[core->reg_sp];
  core->reg_sp++;

  // IP is immediately incremented
  core->reg_ip = address - 1;
}

void patchy_exec_hlt(struct PatchyCore* core) {
  set_core_flag(core, HLT, true);
  core->halted = true;
}
