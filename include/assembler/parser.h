#ifndef _ASM_PARSER_H
#define _ASM_PARSER_H

#include <stdbool.h>

enum PARSER_MODES {
  NEW,
  SEEKING_IDENTIFIER,
  PARSING_IDENTIFIER,
  SEEKING_ARGUMENT,
  PARSING_ARGUMENT
};

bool is_valid_identifier_start(const char c);
bool is_valid_identifier_end(const char c);
bool is_valid_argument_start(const char c);
bool is_valid_argument_end(const char c);
bool is_valid_line_ending(const char c);

#endif
