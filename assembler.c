#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "assembler/parser.h"
#include "assembler/entity.h"
#include "assembler/label_table.h"
#include "assembler/tree.h"

int main(int argc, char *argv[]) {

  // TODO: Use the GNU C args parsing lib
  if(argc == 1) {
    printf("Usage todo, RTFM\n");
    return 1;
  } else if(argc > 2) {
    printf("Only the first argument is used!\n"); // Bah
  }

  FILE* fp = fopen(argv[1], "r");

  if(fp == NULL) {
    printf("Failed to open file for reading: %s", argv[1]);
    return 1;
  }

  char* buffer = NULL;
  size_t buffer_len = 0;

  struct AssemblyLabelTable* prog_label_table = alloc_label_table();
  struct ParsedEntityTree* prog_tree = alloc_entity_tree();

  while(getline(&buffer, &buffer_len, fp) != -1) {
    enum PARSER_MODES mode = NEW;

    struct ParsedEntity* entity = NULL;
    uint16_t entity_start = 0;

    for(uint16_t i = 0; i < buffer_len; i++) {
      bool line_ended = is_valid_line_ending(buffer[i]);

      if(!line_ended) {

        // Starting reading the line, check for identifier
        if(mode == NEW || mode == SEEKING_IDENTIFIER) {
          if(is_valid_identifier_start(buffer[i])) {
            entity_start = i;
            mode = PARSING_IDENTIFIER;
          }

        // Read an identifier, check for any arguments
        } else if(mode == SEEKING_ARGUMENT) {
          if(is_valid_argument_start(buffer[i])) {
            entity_start = i;
            mode = PARSING_ARGUMENT;
          }
        }
      }

      // Reading an identifier, check for completion
      if(mode == PARSING_IDENTIFIER) {
        if(is_valid_identifier_end(buffer[i])) {
          mode = SEEKING_ARGUMENT;

          // Save identifier in a new entity
          assert(entity == NULL);
          entity = alloc_asm_entity();

          entity->identifier_len = (i - entity_start) + 1;
          entity->identifier = malloc(sizeof(char) * entity->identifier_len);

          strncpy(
            entity->identifier,
            buffer + entity_start,
            entity->identifier_len - 1
          );

          entity->identifier[entity->identifier_len - 1] = '\0';

          // Detect labels properly
          if(buffer[i] == ':') {
            entity->is_label = true;
          }
        }

      // Reading an argument, check for completion
      } else if(mode == PARSING_ARGUMENT) {
        if(is_valid_argument_end(buffer[i])) {
          mode = SEEKING_ARGUMENT;

          // Initialise the argument pool if needed
          if(entity->arguments == NULL) {
            alloc_asm_entity_args(entity);

          // Grow the argument pool by 2 if needed
          } else if(entity->alloc_arguments == entity->n_arguments) {
            grow_asm_entity_args(entity);
          }

          uint8_t arg_i = entity->n_arguments;
          entity->argument_len[arg_i] = (i - entity_start) + 1;
          entity->arguments[arg_i] = malloc(
            sizeof(char) * entity->argument_len[arg_i]
          );

          strncpy(
            entity->arguments[arg_i],
            buffer + entity_start,
            entity->argument_len[arg_i] - 1
          );

          entity->arguments[arg_i][entity->argument_len[arg_i] - 1] = '\0';
          entity->n_arguments++;
        }
      }

      if(line_ended) {
        break;
      }
    } // for

    // Deal with a processed entity
    if(entity != NULL) {
      optimise_asm_entity_args(entity);
      add_entity_to_tree(entity, prog_tree);
    }
  }

  resolve_entity_tree_labels(prog_tree, prog_label_table);

  uint16_t prog_size = get_tree_bin_buff_size(prog_tree);
  uint32_t* prog_buff = malloc(sizeof(uint32_t) * prog_size);

  assemble_entity_tree(prog_tree, prog_label_table, prog_buff);

  dealloc_label_table(prog_label_table);
  dealloc_entity_tree(prog_tree);

  free(buffer);
  fclose(fp);

  fp = fopen("test-out.bin", "wb");
  fwrite(prog_buff, sizeof(uint32_t), prog_size, fp);
  fclose(fp);

  printf("Write %d assembled bytes to test-out.bin", prog_size * 4);

  return 0;
}
