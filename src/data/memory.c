#include "data/memory.h"
#include <stdlib.h>

void alloc_memory16(struct Memory16* mem, const uint16_t size) {
  mem->contents = malloc(sizeof(uint16_t) * size);
  mem->size = size;
}

void alloc_memory32(struct Memory32* mem, const uint32_t size) {
  mem->contents = malloc(sizeof(uint32_t) * size);
  mem->size = size;
}

void dealloc_memory16(struct Memory16* mem) {
  free(mem->contents);

  mem->size = 0;
}

void dealloc_memory32(struct Memory32* mem) {
  free(mem->contents);

  mem->size = 0;
}
