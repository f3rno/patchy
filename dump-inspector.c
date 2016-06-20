#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ncurses.h>

#include "render.h"

#define NCURSES false

/**
 * Provides basic dump utilities, allowing one to peek inside of VRAM/RAM blobs
 */
int main(int argc, char *argv[]) {

  // TODO: Use the GNU C args parsing lib
  if(argc == 1) {
    printf("Usage todo, RTFM\n");
    return 1;
  } else if(argc > 2) {
    printf("Only the first argument is used!\n"); // Bah
  }

  // TODO: Everything besides basic VRAM inspection
  uint16_t* vram = malloc(sizeof(uint16_t) * 48);
  uint16_t* in_col = malloc(sizeof(uint16_t));

  FILE* fp = fopen(argv[1], "rb");
  fread(vram, sizeof(uint16_t), 48, fp);
  fclose(fp);

  if(NCURSES) {
    ncurses_init();
    render_led_color_init();

    for(uint8_t y = 0; y < 16; y++) {
      for(uint8_t x = 0; x < 16; x++) {
        uint8_t color = get_vram_led_color_pair(vram, x, y);

        if(color != 0) {
          attron(COLOR_PAIR(color));
          mvaddch(y, (x * 2), ' ' | A_REVERSE);
          mvaddch(y, (x * 2) + 1, ' ' | A_REVERSE);
          attroff(COLOR_PAIR(color));
        }
      }

      printf("\n");
    }

    getch();
    endwin();

  // Basic text representation
  } else {
    for(uint8_t y = 0; y < 16; y++) {
      for(uint8_t x = 0; x < 16; x++) {
        bool r = vram[x] & (1 << y);
        bool g = vram[x + 16] & (1 << y);
        bool b = vram[x + 32] & (1 << y);

        if(r) {
          printf("R");
        } else {
          printf(" ");
        }

        if(g) {
          printf("G");
        } else {
          printf(" ");
        }

        if(b) {
          printf("B ");
        } else {
          printf("  ");
        }
      }

      printf("\n");
    }
  }

  free(in_col);
  free(vram);

  return 0;
}
