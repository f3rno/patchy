#include <string.h>

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

  memset(core->ram->contents, 0, (sizeof(uint16_t) * core->ram->size) / sizeof(unsigned char));
  memset(core->rom->contents, 0, (sizeof(uint32_t) * core->rom->size) / sizeof(unsigned char));
}
