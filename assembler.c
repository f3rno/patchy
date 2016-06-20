#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

enum PARSER_MODES {
  NEW,
  SEEKING_IDENTIFIER,
  PARSING_IDENTIFIER,
  SEEKING_ARGUMENT,
  PARSING_ARGUMENT
};

struct ParsedEntity {
  char* identifier;
  uint8_t identifier_len;

  char** arguments;
  uint8_t* argument_len;
  uint8_t n_arguments;
  uint8_t alloc_arguments;
};

bool is_valid_identifier_start(char c);
bool is_valid_identifier_end(char c);
bool is_valid_argument_start(char c);
bool is_valid_argument_end(char c);

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

  while(getline(&buffer, &buffer_len, fp) != -1) {
    enum PARSER_MODES mode = NEW;

    struct ParsedEntity* entity = NULL;
    uint16_t entity_start = 0;

    for(uint16_t i = 0; i < buffer_len; i++) {

      // Handle line endings, and two types of comments
      bool line_ended = buffer[i] == '\n' ||
                        buffer[i] == '\0' ||
                        buffer[i] == ';' ||
                        buffer[i] == '#';

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
          entity = malloc(sizeof(struct ParsedEntity));
          entity->n_arguments = 0;
          entity->alloc_arguments = 0;
          entity->argument_len = NULL;
          entity->arguments = NULL;

          entity->identifier_len = (i - entity_start) + 1;
          entity->identifier = malloc(sizeof(char) * entity->identifier_len);

          strncpy(
            entity->identifier,
            buffer + entity_start,
            entity->identifier_len - 1
          );

          entity->identifier[entity->identifier_len - 1] = '\0';
        }

      // Reading an argument, check for completion
      } else if(mode == PARSING_ARGUMENT) {
        if(is_valid_argument_end(buffer[i])) {
          mode = SEEKING_ARGUMENT;

          // Initialise the argument pool if needed
          if(entity->arguments == NULL) {
            assert(entity->n_arguments == 0);
            assert(entity->alloc_arguments == 0);

            entity->arguments = malloc(sizeof(char*) * 2);
            entity->argument_len = malloc(sizeof(uint8_t) * 2);
            entity->n_arguments = 0;

          // Grow the argument pool by 2 if needed
          } else if(entity->alloc_arguments == entity->n_arguments) {
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

      // Truncate the argument pool if needed
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

      // Somehow save the entity in a tree of some sort
      // For now, just print information about it to the screen
      // TODO: Code method to print out entity trees for debugging
      printf("  %s\n", entity->identifier);

      for(int i = 0; i < entity->n_arguments; i++) {
        printf("    %s\n", entity->arguments[i]);

        free(entity->arguments[i]);
      }

      free(entity->identifier);
      free(entity->arguments);
      free(entity->argument_len);
      free(entity);
    }
  }

  free(buffer);
  fclose(fp);

  return 0;
}

bool is_valid_identifier_start(char c) {
  return (c >= 'A' && c <= 'Z') ||
         (c >= 'a' && c <= 'z') ||
         c == 0x5F; // _
}

bool is_valid_identifier_end(char c) {
  return !is_valid_identifier_start(c);
}

bool is_valid_argument_start(char c) {
  return (c >= '0' && c <= '9') ||
         (c >= 'A' && c <= 'Z') ||
         (c >= 'a' && c <= 'z') ||
          c == 0x5B || c == 0x5D || // [ ]
          c == 0x5F; // _
}

bool is_valid_argument_end(char c) {
  return !is_valid_argument_start(c);
}
