#ifndef _ASM_ENTITY_H
#define _ASM_ENTITY_H

#include <stdint.h>
#include <stdbool.h>

struct AssemblyLabelTable;

struct ParsedEntity {
  char* identifier;
  uint8_t identifier_len;

  char** arguments;
  uint8_t* argument_len;
  uint8_t n_arguments;
  uint8_t alloc_arguments;

  bool is_label;
};

struct ParsedEntity* alloc_asm_entity();
void dealloc_asm_entity(struct ParsedEntity* entity);
void alloc_asm_entity_args(struct ParsedEntity* entity);
void grow_asm_entity_args(struct ParsedEntity* entity);
void optimise_asm_entity_args(struct ParsedEntity* entity);
void print_asm_entity(const struct ParsedEntity* entity);

void assemble_entity(
  const struct ParsedEntity* entity,
  const struct AssemblyLabelTable* label_table,
  uint32_t* instruction
);

#endif
