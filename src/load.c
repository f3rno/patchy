#include <stdio.h>
#include <assert.h>

#include "load.h"
#include "data/memory.h"

uint16_t load_file_into_rom(const char* filename, struct Memory32* rom) {

  // TODO: Test for file existence, permissions, return error, etc
  FILE* fp = fopen(filename, "rb");

  // Get filesize for buffer allocation
  fseek(fp, 0, SEEK_END);
  uint16_t num_bytes = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  assert(num_bytes % 4 == 0); // Must be divisible by 4, 32bit instructions
  assert((num_bytes / 4) < rom->size); // Can we fit it?

  size_t bytes_read = fread(rom->contents, 1, num_bytes, fp);

  assert((uint16_t)bytes_read == num_bytes);

  fclose(fp);

  return num_bytes;
}
