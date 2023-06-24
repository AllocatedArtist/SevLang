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

typedef struct AST AST;
#define MAX_PARAMETERS 10

struct AST {
  enum {
    AST_PROGRAM,
    AST_VARIABLE,
    AST_VARIABLE_DECLARATION,
    AST_VARIABLE_ASSIGNMENT,
    AST_ASSIGNMENT_R_VAL,
    AST_NUMBER_LITERAL,
    AST_FUNCTION_DECLARATION,
    AST_CALL_EXPRESSION,
    AST_BINARY_OPERATION,
    AST_END_LINE,
    AST_COMMENT,
  } type;
  union {
    struct AST_COMMENT { const char* comment; } AST_COMMENT;
    struct AST_PROGRAM { AST* body; } AST_PROGRAM;
    struct AST_VARIABLE_DECLARATION { const char* type; const char* variable_name; } AST_VARIABLE_DECLARATION;
    struct AST_ASSIGNMENT_R_VAL { const char* assignment; } AST_ASSIGNMENT_R_VAL; //imma cheat lol
    struct AST_VARIABLE_ASSIGNMENT { const char* variable; } AST_VARIABLE_ASSIGNMENT;
    struct AST_VARIABLE { const char* variable_name; } AST_VARIABLE;
    struct AST_NUMBER_LITERAL { const char* literal; } AST_NUMBER_LITERAL; 
    struct AST_FUNCTION_DECLARATION { 
      int num_parameters; 
      const char* type; 
      const char* name; 
      AST* parameters[MAX_PARAMETERS];
      AST* return_variable;
      AST* variable_assignment;
    } AST_FUNCTION_DECLARATION;
    struct AST_BINARY_OPERATION { const char* op; } AST_BINARY_OPERATION;
    struct AST_END_LINE { const char* value; const char* return_variable; } AST_END_LINE;
  } data;
};

struct {
  bool is_set;
  const char* return_variable;
} static current_function_return; //no pleasure in doing this

typedef struct ast_list_t {
  AST* current;
  struct ast_list_t* next;
} ast_list_t;

AST* new_node(AST ast);
void free_ast_node(AST* ast);
void push_ast_list(ast_list_t** list, AST* node);
void print_ast(AST* ast);
void print_ast_list(ast_list_t* list);
void free_ast_list(ast_list_t** list);

#define GET_AST_DATA(ast, tag) ast->data.tag 
#define AST_NODE(tag, ...) \
  new_node((AST) { tag, { .tag = (struct tag) { __VA_ARGS__ } } }) \


enum TokenType {
  NoneType, //Just a placeholder
  FnType,
  LetType,
  OpenParenthesisType,
  ClosedParenthesisType,
  AssignmentOperatorType,
  TypeIdentifierType,
  ColonAssignmentType,
  OpenBracketType,
  ClosedBracketType,
  SemiColonType,
  EndType,
  IdentifierType,
  CommentType,
  BinaryOperationType,
  NumberType,
  CommaType,
  CallType,
  BeingLazyYay,
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


ast_list_t* produce_ast(token_list_t* tokens_list);

struct {
  const char* keyword; 
  enum TokenType type;
} static keywords[] = {
  { "let", LetType }, 
  { "int", TypeIdentifierType },
  { "fn", FnType },
  { "end", EndType },
  { "float", TypeIdentifierType },
  { "void", TypeIdentifierType },
  { "call", CallType },
};


const char* init_sev(int arglen, char** arguments);

source_file_t load_source_file(const char* filename);
void free_source_file(source_file_t* source_file) ;

void remove_whitespace(char* trimmed, const char* untrimmed); 

void list_add_token(token_list_t** list, token_t value);
void list_free_all_tokens(token_list_t** list); 
token_t create_token(enum TokenType type, const char* value, size_t len);
token_t tokenize(char* line, size_t size, size_t* counter, char** identifier); 
void print_lexer(token_list_t* tokens_list);

void write_ast_to_file(FILE* stream, ast_list_t* list);
#endif
