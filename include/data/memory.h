#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>
#include <stdbool.h>

struct Memory16 {
  uint16_t* contents;
  uint16_t size;
  bool read_only;
};

struct Memory32 {
  uint32_t* contents;
  uint32_t size;
  bool read_only;
};

void alloc_memory16(struct Memory16* mem, const uint16_t size);
void alloc_memory32(struct Memory32* mem, const uint32_t size);
void dealloc_memory16(struct Memory16* mem);
void dealloc_memory32(struct Memory32* mem);

#endif
