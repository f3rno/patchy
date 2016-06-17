#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "data/instruction.h"
#include "core.h"

int main(int argc, char *argv[]) {

  /**
   * TODO
   *
   * - Assembler
   * - Actual arguments; support assembly/disassembly/execution
   * - Basically everything else
   */

  PatchyCore* core = malloc(sizeof(PatchyCore));

  alloc_patchy_core(core, 0x10000, 0x10000); // ~64k RAM/ROM
  clear_patchy_core(core);

  dealloc_patchy_core(core);

  return 0;
}
