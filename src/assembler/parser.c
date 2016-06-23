#include "assembler/parser.h"

bool is_valid_identifier_start(const char c) {
  return (c >= 'A' && c <= 'Z') ||
         (c >= 'a' && c <= 'z') ||
         c == 0x5F; // _
}

bool is_valid_identifier_end(const char c) {
  return !is_valid_identifier_start(c);
}

bool is_valid_argument_start(const char c) {
  return (c >= '0' && c <= '9') ||
         (c >= 'A' && c <= 'Z') ||
         (c >= 'a' && c <= 'z') ||
          c == 0x5B || c == 0x5D || // [ ]
          c == 0x5F; // _
}

bool is_valid_argument_end(const char c) {
  return !is_valid_argument_start(c);
}

// Handle line endings, and two types of comments
bool is_valid_line_ending(const char c) {
  return c == '\n' || c == '\0' || c == ';' || c == '#';
}
