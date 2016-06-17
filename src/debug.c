#include <stdio.h>
#include <stdint.h>

#include "debug.h"
#include "core.h"
#include "data/memory.h"

// TODO: Left-pad words
void print_memory32(struct Memory32* mem) {
  printf("32bit memory dump (%d words)\n", mem->size);

  for(uint32_t address = 0; address < mem->size; address++) {
    if(address == 0 || address % 4 == 0) {
      printf("\n0x%x: ", address);
    }

    printf(" %x", mem->contents[address]);
  }

  printf("\n");
}

// TODO: Left-pad words
void print_coredump(struct PatchyCore* core) {
  printf("Generating coredump:\n");
  printf("  Registers\n");
  printf("    a: 0x%x\n", core->reg_a);
  printf("    b: 0x%x\n", core->reg_b);
  printf("    c: 0x%x\n", core->reg_c);
  printf("    d: 0x%x\n", core->reg_d);
  printf("    e: 0x%x\n", core->reg_e);
  printf("    f: 0x%x\n", core->reg_f);
  printf("\n");
  printf("   px: 0x%x\n", core->reg_px);
  printf(" flgs: 0x%x\n", core->reg_flgs);
  printf("\n");
  printf("  in1: 0x%x\n", core->reg_in1);
  printf("  in2: 0x%x\n", core->reg_in2);
  printf(" out1: 0x%x\n", core->reg_out1);
  printf(" out2: 0x%x\n", core->reg_out2);
  printf("\n");
  printf("   dp: 0x%x\n", core->reg_dp);
  printf("   ip: 0x%x\n", core->reg_ip);
  printf("  ret: 0x%x\n", core->reg_ret);
  printf("   sp: 0x%x\n", core->reg_sp);
}
