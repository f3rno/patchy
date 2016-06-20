#ifndef _RENDER_H
#define _RENDER_H

#include <stdint.h>
#include <ncurses.h>

struct PatchyRenderData {
  WINDOW* led_matrix_win;
  WINDOW* lcd_win;
  WINDOW* debug_win;
};

void ncurses_init();

void render_init(struct PatchyRenderData* data);
void render_led_color_init();
void render_vram_led_matrix(struct PatchyRenderData* data, uint16_t* vram);
void render_lcd(struct PatchyRenderData* data);
void render_cleanup(struct PatchyRenderData* data);

uint8_t get_vram_led_color_pair(uint16_t* vram, uint8_t x, uint8_t y);

#endif
