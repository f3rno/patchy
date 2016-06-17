#ifndef _LOAD_H
#define _LOAD_H

#include <stdint.h>

struct Memory32;

uint16_t load_file_into_rom(const char* filename, struct Memory32* rom);

#endif
