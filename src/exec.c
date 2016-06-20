#include <stdbool.h>
#include <stdio.h>

#include "arch.h"
#include "exec.h"
#include "core.h"
#include "render.h"
#include "data/memory.h"
#include "data/instruction.h"
#include "disassembly.h"

void patchy_exec(struct PatchyCore* core, struct PatchyRenderData* render_data) {
  bool done = false;

  struct Instruction instruction;
  //char i_buff[1024];

  while(!done && !core->halted) {
    decode_instruction(&core->rom->contents[core->reg_ip], &instruction);

    // TODO: Add debug flag
    //disassemble_instruction(&instruction, &i_buff);
    //printf("0x%x:  %s\n", core->reg_ip, i_buff);

    patchy_exec_instruction(core, &instruction);

    // SPX causes VRAM re-render
    if(render_data != NULL && instruction.opcode == OP_SPX) {
      render_vram_led_matrix(render_data, core->vram->contents);
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

void patchy_exec_instruction(struct PatchyCore* core, struct Instruction* ins) {
  switch(ins->opcode) {
    case OP_MOV:
      patchy_exec_mov(core, ins);
      break;

    case OP_LDI:
      patchy_exec_ldi(core, ins);
      break;

    case OP_LDM:
      patchy_exec_ldm(core, ins);
      break;

    case OP_LPX:
      patchy_exec_lpx(core, ins);
      break;

    case OP_SPX:
      patchy_exec_spx(core, ins);
      break;

    case OP_STR:
      patchy_exec_str(core, ins);
      break;

    case OP_PUSH:
      patchy_exec_push(core, ins);
      break;

    case OP_POP:
      patchy_exec_pop(core, ins);
      break;

    case OP_ADD:
      patchy_exec_add(core, ins);
      break;

    case OP_ADDI:
      patchy_exec_addi(core, ins);
      break;

    case OP_SUB:
      patchy_exec_sub(core, ins);
      break;

    case OP_SUBI:
      patchy_exec_subi(core, ins);
      break;

    case OP_CMP:
      patchy_exec_cmp(core, ins);
      break;

    case OP_AND:
      patchy_exec_and(core, ins);
      break;

    case OP_OR:
      patchy_exec_or(core, ins);
      break;

    case OP_XOR:
      patchy_exec_xor(core, ins);
      break;

    case OP_SHL:
      patchy_exec_shl(core, ins);
      break;

    case OP_SHR:
      patchy_exec_shr(core, ins);
      break;

    case OP_JMP:
      patchy_exec_jmp(core, ins);
      break;

    case OP_JE:
      patchy_exec_je(core, ins);
      break;

    case OP_JNE:
      patchy_exec_jne(core, ins);
      break;

    case OP_JG:
      patchy_exec_jg(core, ins);
      break;

    case OP_JGE:
      patchy_exec_jge(core, ins);
      break;

    case OP_JL:
      patchy_exec_jl(core, ins);
      break;

    case OP_JLE:
      patchy_exec_jle(core, ins);
      break;

    case OP_JZ:
      patchy_exec_jz(core, ins);
      break;

    case OP_JNZ:
      patchy_exec_jnz(core, ins);
      break;

    case OP_CALL:
      patchy_exec_call(core, ins);
      break;

    case OP_CALLI:
      patchy_exec_calli(core, ins);
      break;

    case OP_RET:
      patchy_exec_ret(core);
      break;

    case OP_HLT:
      patchy_exec_hlt(core);
      break;

    // Noop
    case OP_NOOP:
      break;

    default:
      printf("[0x%x]: Unknown op: 0x%x\n", core->reg_ip, ins->opcode);
      break;
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
  /**
   * PX stores the pixel address in the form (y * 16) + x
   * VRAM stores pixel data in three chunks of 16 addresses. I.e:
   * RRRRRRRRRRRRRRRRGGGGGGGGGGGGGGGGBBBBBBBBBBBBBBBB
   * Each character represents one column of LEDs ^
   *
   * Data in the color register is stored in the form 0bRGB
   */

  uint16_t color = *get_core_reg_by_adr(core, i->src);
  uint8_t col = core->reg_px / 16;
  uint8_t row = core->reg_px % 16;

  if((color | 0b1) > 0) { // Red
    core->vram->contents[col] |= 1 << row;
  } else {
    core->vram->contents[col] &= ~(1 << row);
  }

  if((color | 0b10) > 0) { // Green
    core->vram->contents[16 + col] |= 1 << row;
  } else {
    core->vram->contents[16 + col] &= ~(1 << row);
  }

  if((color | 0b100) > 0) { // Blue
    core->vram->contents[32 + col] |= 1 << row;
  } else {
    core->vram->contents[32 + col] &= ~(1 << row);
  }
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
