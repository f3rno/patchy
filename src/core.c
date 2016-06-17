#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "arch.h"
#include "core.h"
#include "data/memory.h"

void alloc_patchy_core(struct PatchyCore* core, const uint16_t ram_size, const uint16_t rom_size) {
  core->ram = malloc(sizeof(struct Memory16));
  core->rom = malloc(sizeof(struct Memory32));

  alloc_memory16(core->ram, ram_size);
  alloc_memory32(core->rom, rom_size);
}

void dealloc_patchy_core(struct PatchyCore* core) {
  dealloc_memory16(core->ram);
  dealloc_memory32(core->rom);

  free(core->ram);
  free(core->rom);
}

void clear_patchy_core(struct PatchyCore* core) {
  core->reg_a = 0;
  core->reg_b = 0;
  core->reg_c = 0;
  core->reg_d = 0;
  core->reg_e = 0;
  core->reg_f = 0;

  core->reg_px = 0;
  core->reg_flgs = 0;

  core->reg_in1 = 0;
  core->reg_in2 = 0;
  core->reg_out1 = 0;
  core->reg_out2 = 0;

  core->reg_dp = 0;
  core->reg_ip = 0;
  core->reg_ret = 0;
  core->reg_sp = 0;

  core->halted = false;

  memset(core->ram->contents, 0, (sizeof(uint16_t) * core->ram->size) / sizeof(unsigned char));
  memset(core->rom->contents, 0, (sizeof(uint32_t) * core->rom->size) / sizeof(unsigned char));
}

uint16_t* get_core_reg_by_adr(struct PatchyCore* core, uint8_t address) {
  switch(address) {
    case 0x0: return &core->reg_a;
    case 0x1: return &core->reg_b;
    case 0x2: return &core->reg_c;
    case 0x3: return &core->reg_d;
    case 0x4: return &core->reg_e;
    case 0x5: return &core->reg_f;
    case 0x6: return &core->reg_px;
    case 0x7: return &core->reg_flgs;
    case 0x8: return &core->reg_in1;
    case 0x9: return &core->reg_in2;
    case 0xa: return &core->reg_out1;
    case 0xb: return &core->reg_out2;
    case 0xc: return &core->reg_dp;
    case 0xd: return &core->reg_ip;
    case 0xe: return &core->reg_ret;
    case 0xf: return &core->reg_sp;

    default:
      assert(false);
      return NULL;
  }
}

void set_core_flag(struct PatchyCore* core, enum PATCHY_FLAG flag, bool value) {
  uint8_t bit = 0;

  switch(flag) {
    case LT:
      bit = 0;
      break;

    case GT:
      bit = 1;
      break;

    case EQ:
      bit = 2;
      break;

    case HLT:
      bit = 3;
      break;

    case ZE:
      bit = 4;
      break;

    // Drop out on unknown flags
    default:
      return;
  }

  if(value) {
    core->reg_flgs |= 1 << bit;
  } else {
    core->reg_flgs &= ~(1 << bit);
  }
}

bool get_core_flag(struct PatchyCore* core, enum PATCHY_FLAG flag) {
  switch(flag) {
    case LT: return (core->reg_flgs & 0b1) > 0;
    case GT: return (core->reg_flgs & 0b10) > 0;
    case EQ: return (core->reg_flgs & 0b100) > 0;
    case HLT: return (core->reg_flgs & 0b1000) > 0;
    case ZE: return (core->reg_flgs & 0b10000) > 0;

    default:
      return false;
  }
}
