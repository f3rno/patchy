#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <assert.h>

#include "data/memory.h"
#include "core.h"
#include "load.h"
#include "exec.h"
#include "render.h"
#include "debug.h"

// TODO: Move to arguments
#define HEADLESS false
#define DUMP_VRAM false
#define VRAM_DUMP_FILENAME "vram-dump.bin"

void draw_vram_led_matrix(WINDOW* win, uint16_t* vram);

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

  // Setup ncurses
  struct PatchyRenderData* render_data = NULL;

  if(!HEADLESS) {
    render_data = malloc(sizeof(struct PatchyRenderData));
    render_init(render_data);
  }

  // Init core
  struct PatchyCore* core = malloc(sizeof(struct PatchyCore));

  alloc_patchy_core(core, 0xFFFF, 0xFFFF); // ~64k RAM/ROM
  clear_patchy_core(core);

  uint16_t bytes_loaded = load_file_into_rom(argv[1], core->rom);

  printf("Loaded %s (%i bytes), executing...\n\n", argv[1], bytes_loaded);

  // Set up stack pointer (end of RAM) and execute
  core->reg_sp = core->ram->size;
  patchy_exec(core, render_data);

  if(!HEADLESS) {
    getch();
  }

  printf("\n");
  print_coredump(core);

  if(!HEADLESS && render_data != NULL) {
    render_cleanup(render_data);
  }

  // Dump things if needed
  if(DUMP_VRAM) {
    FILE* vram_dump_fp = fopen(VRAM_DUMP_FILENAME, "wb");

    fwrite(core->vram->contents, sizeof(uint16_t), 48, vram_dump_fp);
    fclose(vram_dump_fp);

    printf("Wrote VRAM dump (96 bytes) to %s\n", VRAM_DUMP_FILENAME);
  }

  // Clean up
  dealloc_patchy_core(core);

  return 0;
}
