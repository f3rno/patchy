#ifndef _ASM_LABEL_TABLE
#define _ASM_LABEL_TABLE

#include <stdint.h>
#include <stdbool.h>

struct AssemblyLabel;

struct AssemblyLabelTable {
  struct AssemblyLabel** labels;
  uint16_t count;
  uint16_t alloc_count;
};

struct AssemblyLabelTable* alloc_label_table();
void grow_label_table(struct AssemblyLabelTable* table);
void dealloc_label_table(struct AssemblyLabelTable* table);
void print_label_table(const struct AssemblyLabelTable* table);

bool table_has_label_str(
  const struct AssemblyLabelTable* table,
  const char* label
);

void add_label_to_table(
  struct AssemblyLabelTable* table,
  struct AssemblyLabel* label
);

#endif
