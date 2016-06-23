#include "assembler/label_table.h"
#include "assembler/label.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct AssemblyLabelTable* alloc_label_table() {
  struct AssemblyLabelTable* table = malloc(sizeof(struct AssemblyLabelTable));

  table->labels = NULL;
  table->count = 0;
  table->alloc_count = 0;

  return table;
}

void grow_label_table(struct AssemblyLabelTable* table) {
  table->alloc_count += 2;
  table->labels = realloc(
    table->labels,
    sizeof(struct AssemblyLabel*) * table->alloc_count
  );
}

void dealloc_label_table(struct AssemblyLabelTable* table) {
  free(table->labels);
  free(table);
}

void print_label_table(const struct AssemblyLabelTable* table) {
  for(uint16_t i = 0; i < table->count; i++) {
    printf("0x%x: %s\n", table->labels[i]->address, table->labels[i]->label);
  }
}

bool table_has_label_str(
  const struct AssemblyLabelTable* table,
  const char* label
) {
  for(uint16_t i = 0; i < table->count; i++) {
    if(strcmp(label, table->labels[i]->label) == 0) {
      return true;
    }
  }

  return false;
}

void add_label_to_table(
  struct AssemblyLabelTable* table,
  struct AssemblyLabel* label
) {
  if(table->count == table->alloc_count) {
    grow_label_table(table);
  }

  table->labels[table->count] = label;
  table->count++;
}
