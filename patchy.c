#include <stdio.h>
#include <stdlib.h>

#include "data/memory.h"
#include "core.h"
#include "load.h"
#include "exec.h"
#include "debug.h"

int main(int argc, char *argv[]) {

  // TODO: Use the GNU C args parsing lib
  if(argc == 1) {
    printf("Usage todo, RTFM\n");
    return 1;
  } else if(argc > 2) {
    printf("Only the first argument is used!\n"); // Bah
  }

  /**
   * TODO
   *
   * - Assembler
   * - Actual arguments; support assembly/disassembly/execution
   * - Basically everything else
   */

   // Init core
  struct PatchyCore* core = malloc(sizeof(struct PatchyCore));

  alloc_patchy_core(core, 0xFFFF, 0xFFFF); // ~64k RAM/ROM
  clear_patchy_core(core);

  uint16_t bytes_loaded = load_file_into_rom(argv[1], core->rom);

  printf("Loaded %s (%i bytes), executing...\n\n", argv[1], bytes_loaded);

  // Set up stack pointer (end of RAM) and execute
  core->reg_sp = core->ram->size;
  patchy_exec(core);

  printf("\n");
  print_coredump(core);

  // Clean up
  dealloc_patchy_core(core);

  return 0;
}
