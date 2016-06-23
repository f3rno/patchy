#include "assembler/tree.h"
#include "assembler/entity.h"
#include "assembler/label.h"
#include "assembler/label_table.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct ParsedEntityTree* alloc_entity_tree() {
  struct ParsedEntityTree* tree = malloc(sizeof(struct ParsedEntityTree*));

  tree->entities = NULL;
  tree->count = 0;
  tree->alloc_count = 0;

  return tree;
}

void grow_entity_tree(struct ParsedEntityTree* tree) {
  tree->alloc_count += 8;
  tree->entities = realloc(
    tree->entities,
    sizeof(struct ParsedEntity*) * tree->alloc_count
  );
}

void dealloc_entity_tree(struct ParsedEntityTree* tree) {
  if(tree->count > 0) {
    for(uint16_t i = 0; i < tree->count; i++) {
      dealloc_asm_entity(tree->entities[i]);
    }

    tree->count = 0;
  }

  free(tree->entities);
  free(tree);
}

void add_entity_to_tree(
  struct ParsedEntity* entity,
  struct ParsedEntityTree* tree
) {
  if(tree->alloc_count == tree->count) {
    grow_entity_tree(tree);
  }

  tree->entities[tree->count] = entity;
  tree->count++;
}

/**
 * Sets up a label table backed by the provided entity tree
 */
void resolve_entity_tree_labels(
  const struct ParsedEntityTree* tree,
  struct AssemblyLabelTable* table
) {
  uint16_t address = 0;

  for(uint16_t i = 0; i < tree->count; i++) {
    struct ParsedEntity* entity = tree->entities[i];

    if(entity->is_label) {
      assert(!table_has_label_str(table, entity->identifier)); // Dup label

      struct AssemblyLabel* label = malloc(sizeof(struct AssemblyLabel*));

      label->address = address;
      label->label = malloc(sizeof(char) * entity->identifier_len);

      strcpy(label->label, entity->identifier);

      add_label_to_table(table, label);
    } else {
      address++;
    }
  }
}

uint16_t get_tree_bin_buff_size(const struct ParsedEntityTree* tree) {
  uint16_t size = 0;

  for(uint16_t i = 0; i < tree->count; i++) {
    if(!tree->entities[i]->is_label) {
      size++;
    }
  }

  return size;
}

void assemble_entity_tree(
  const struct ParsedEntityTree* tree,
  const struct AssemblyLabelTable* label_table,
  uint32_t* buff
) {
  uint16_t address = 0;

  for(uint16_t i = 0; i < tree->count; i++) {
    if(!tree->entities[i]->is_label) {
      assemble_entity(tree->entities[i], label_table, buff + address);

      address++;
    }
  }
}
