#ifndef _ASM_TREE
#define _ASM_TREE

#include <stdint.h>

struct ParsedEntity;
struct AssemblyLabelTable;

struct ParsedEntityTree {
  struct ParsedEntity** entities;
  uint16_t count;
  uint16_t alloc_count;
};

struct ParsedEntityTree* alloc_entity_tree();
void grow_entity_tree(struct ParsedEntityTree* tree);
void dealloc_entity_tree(struct ParsedEntityTree* tree);
uint16_t get_tree_bin_buff_size(const struct ParsedEntityTree* tree);

void add_entity_to_tree(
  struct ParsedEntity* entity,
  struct ParsedEntityTree* tree
);

void resolve_entity_tree_labels(
  const struct ParsedEntityTree* tree,
  struct AssemblyLabelTable* table
);

void assemble_entity_tree(
  const struct ParsedEntityTree* tree,
  const struct AssemblyLabelTable* label_table,
  uint32_t* buff
);

#endif
