#ifndef _ASM_LABEL
#define _ASM_LABEL

#include <stdint.h>

struct AssemblyLabel {
  char* label;
  uint16_t address;
};

#endif
