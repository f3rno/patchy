#ifndef _CORE_H
#define _CORE_H

#include <stdint.h>

struct Memory16;
struct Memory32;

struct PatchyCore {
  uint16_t reg_a;
  uint16_t reg_b;
  uint16_t reg_c;
  uint16_t reg_d;
  uint16_t reg_e;
  uint16_t reg_f;

  uint16_t reg_px;
  uint16_t reg_flgs;

  uint16_t reg_in1;
  uint16_t reg_in2;
  uint16_t reg_out1;
  uint16_t reg_out2;

  uint16_t reg_dp;
  uint16_t reg_ip;
  uint16_t reg_ret;
  uint16_t reg_sp;

  struct Memory16* ram;
  struct Memory32* rom;
};

void alloc_patchy_core(struct PatchyCore* core, const uint16_t ram_size, const uint16_t rom_size);
void dealloc_patchy_core(struct PatchyCore* core);
void clear_patchy_core(struct PatchyCore* core);

#endif
