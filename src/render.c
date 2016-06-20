#include <assert.h>
#include <stdio.h>

#include "render.h"

void ncurses_init() {
  initscr();
  start_color();

  if(!can_change_color()) {
    printf("Your term doesn't support color changes, aborting");
    assert(false);
  }
}

void render_led_color_init() {
  init_color(1, 1000, 0, 0);       // R
  init_color(2, 0, 1000, 0);       // G
  init_color(3, 0, 0, 1000);       // B
  init_color(4, 1000, 1000, 0);    // RG
  init_color(5, 1000, 0, 1000);    // RB
  init_color(6, 0, 1000, 1000);    // GB
  init_color(7, 1000, 1000, 1000); // RGB

  init_pair(1, 1, COLOR_BLACK);
  init_pair(2, 2, COLOR_BLACK);
  init_pair(3, 3, COLOR_BLACK);
  init_pair(4, 4, COLOR_BLACK);
  init_pair(5, 5, COLOR_BLACK);
  init_pair(6, 6, COLOR_BLACK);
  init_pair(7, 7, COLOR_BLACK);
}

void render_init(struct PatchyRenderData* data) {
  ncurses_init();

  data->led_matrix_win = newwin(19, 34, 1, 2);
  data->lcd_win = newwin(7, 22, 1, 38);
  data->debug_win = newwin(10, 22, 21, 2);

  box(data->led_matrix_win, '*', '*');
  box(data->lcd_win, '*', '*');
  box(data->debug_win, '*', '*');

  touchwin(data->led_matrix_win);
  touchwin(data->lcd_win);
  touchwin(data->debug_win);

  wrefresh(data->led_matrix_win);
  wrefresh(data->lcd_win);
  wrefresh(data->debug_win);

  refresh();
}

void render_cleanup(struct PatchyRenderData* data) {
  delwin(data->led_matrix_win);
  delwin(data->lcd_win);
  delwin(data->debug_win);
  endwin();
}

void render_vram_led_matrix(struct PatchyRenderData* data, uint16_t* vram) {
  WINDOW* win = data->led_matrix_win;

  mvwprintw(win, 1, 9, "-- LED Matrix --");

  render_led_color_init();

  for(uint8_t column = 0; column < 16; column++) {

    // Calculate pixel color
    for(uint8_t row = 0; row < 16; row++) {
      uint8_t color = get_vram_led_color_pair(vram, column, row);

      if(color != 0) {
        // TODO: Track current color pair, switch on/off only when needed
        // TODO: Only update changed pixels
        wattron(win, COLOR_PAIR(color));
        mvwaddch(win, row, (column * 2), ' ' | A_REVERSE);
        mvwaddch(win, row, (column * 2) + 1, ' ' | A_REVERSE);
        wattroff(win, COLOR_PAIR(color));
      } else {
        mvwaddch(win, row, (column * 2), ' ');
        mvwaddch(win, row, (column * 2) + 1, ' ');
      }
    }
  }

  wrefresh(data->led_matrix_win);
}

uint8_t get_vram_led_color_pair(uint16_t* vram, uint8_t x, uint8_t y) {
  uint8_t color = 0;

  if((vram[x] & (1 << y)) > 0) {
    color += 1;
  }

  if((vram[x + 16] & (1 << y)) > 0) {
    color += 5;
  }

  if((vram[x + 32] & (1 << y)) > 0) {
    color += 10;
  }

  /**
   * Where R = 1, G = 5, B = 10
   *
   *   = 0
   * R = 1
   * G = 5
   * B = 10
   * RG = 6
   * RB = 11
   * GB = 15
   * RGB = 16
   */
  switch(color) {
    case 1: return 1;
    case 5: return 2;
    case 10: return 3;
    case 6: return 4;
    case 11: return 5;
    case 15: return 6;
    case 16: return 7;

    case 0:
    default:
      return 0;
  }
}
