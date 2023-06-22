#ifndef SEV_H
#define SEV_H

#include "basic.h"

typedef struct {
  char* file_contents;
  size_t file_size;
} source_file_t;

typedef enum { //just use stdbool bro
  false,
  true
} bool;

enum TokenType {
  NoneType, //Just a placeholder
  FnType,
  LetType,
  OpenParenthesisType,
  ClosedParenthesisType,
  AssignmentOperatorType,
  IntType,
  ColonAssignmentType,
  OpenBracketType,
  ClosedBracketType,
  SemiColonType,
  EndType,
  IdentifierType,
  CommentType,
  BinaryOperationType,
  NumberType,
};

typedef struct {
  enum TokenType type;
  char* value;
  int strlen;
} token_t;

typedef struct token_list {
  token_t current;
  struct token_list* next;
} token_list_t;

const char* init_sev(int arglen, char** arguments);

source_file_t load_source_file(const char* filename);
void free_source_file(source_file_t* source_file) ;

void remove_whitespace(char* trimmed, const char* untrimmed); 

void list_add_token(token_list_t** list, token_t value);
void list_free_all_tokens(token_list_t** list); 
token_t create_token(enum TokenType type, const char* value, size_t len);
token_t tokenize(char* line, size_t size, size_t* counter, char** identifier); 
void print_lexer(token_list_t* tokens_list);

#endif
