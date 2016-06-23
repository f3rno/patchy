#include "assembler/entity.h"
#include "assembler/label_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct ParsedEntity* alloc_asm_entity() {
  struct ParsedEntity* entity = malloc(sizeof(struct ParsedEntity));

  entity->n_arguments = 0;
  entity->alloc_arguments = 0;
  entity->argument_len = NULL;
  entity->arguments = NULL;
  entity->identifier_len = 0;
  entity->identifier = NULL;
  entity->is_label = false;

  return entity;
}

void dealloc_asm_entity(struct ParsedEntity* entity) {
  free(entity->identifier);

  if(entity->arguments != NULL) {
    for(uint8_t i = 0; i < entity->n_arguments; i++) {
      free(entity->arguments[i]);
    }
  }

  free(entity->arguments);
  free(entity->argument_len);
  free(entity);
}

void alloc_asm_entity_args(struct ParsedEntity* entity) {
  assert(entity->n_arguments == 0);
  assert(entity->alloc_arguments == 0);

  entity->arguments = malloc(sizeof(char*) * 2);
  entity->argument_len = malloc(sizeof(uint8_t) * 2);
  entity->n_arguments = 0;
}

void grow_asm_entity_args(struct ParsedEntity* entity) {
  entity->arguments = realloc(
    entity->arguments,
    sizeof(char*) * (entity->alloc_arguments + 2)
  );

  entity->argument_len = realloc(
    entity->argument_len,
    sizeof(uint8_t) * (entity->alloc_arguments + 2)
  );

  entity->alloc_arguments += 2;
}

void optimise_asm_entity_args(struct ParsedEntity* entity) {
  if(entity->alloc_arguments > entity->n_arguments) {
    entity->arguments = realloc(
      entity->arguments,
      sizeof(char*) * entity->n_arguments
    );

    entity->argument_len = realloc(
      entity->argument_len,
      sizeof(uint8_t) * entity->n_arguments
    );

    entity->alloc_arguments = entity->n_arguments;
  }
}

void print_asm_entity(const struct ParsedEntity* entity) {
  printf("%s", entity->identifier);

  if(entity->is_label) {
    printf(": ");
  } else {
    printf(" ");
  }

  for(int i = 0; i < entity->n_arguments; i++) {
    printf("%s", entity->arguments[i]);

    if(i != entity->n_arguments - 1) {
      printf(", ");
    }
  }

  printf("\n");
}

void assemble_entity(
  const struct ParsedEntity* entity,
  const struct AssemblyLabelTable* label_table,
  uint32_t* instruction
) {
  *instruction = 0x00000000;
}
