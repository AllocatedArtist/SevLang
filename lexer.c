#include "sev.h"

struct {
  const char* keyword; 
  enum TokenType type;
} keywords[] = {
  { "let", LetType }, 
};


void remove_whitespace(char* trimmed, const char* untrimmed) { 
  while (*untrimmed != '\0') {
   if (!isspace(*untrimmed)) {
    *trimmed = *untrimmed;
    trimmed++;
   }
   untrimmed++;
  }
  *trimmed = '\0';
}

void list_add_token(token_list_t** list, token_t value) { 
  token_list_t* new_val = (token_list_t*)malloc(sizeof(token_list_t));
  token_list_t* last_node = *list;

  if (new_val == NULL) {
    printf("Unable to allocate new token\n");
    return;
  }

  new_val->current = value;
  new_val->next = NULL;

  if (*list == NULL) {
    *list = new_val;
    return;
  }

  while (last_node->next != NULL) last_node = last_node->next;
  last_node->next = new_val;
 }

void list_free_all_tokens(token_list_t** list) {
 token_list_t* current = *list; 
 token_list_t* temp = NULL;
 while (current != NULL) {
  temp = current;
  current = current->next;
  free(temp->current.value);
  free(temp);
 }

 *list = NULL;
}

token_t create_token(enum TokenType type, const char* value, size_t len) {
  char* val = malloc(sizeof(char) * len);
  if (val == NULL) {
    printf("Unable to allocate token value string!\n");
    return (token_t) { NoneType, NULL, 0 };
  }
  strcpy_s(val, len + 1, value);
  return (token_t) { type, val, len };
}

token_t tokenize(char* line, size_t size, size_t* counter, char** identifier) {
  enum TokenType type = NoneType;
  const char* value = NULL;
  size_t token_len = 0;
  int i = *counter;
  static int word_counter = 0;
 
  if (line[i] == '(') {
    type = OpenParenthesisType;
    value = "(";
    token_len = 1;
  } else if (line[i] == ')') {
    type = ClosedParenthesisType;
    value = ")";
    token_len = 1;
  } else if (line[i] == '+' || 
            line[i] == '-'  || 
            line[i] == '*'  || 
            line[i] == '/') {

    type = BinaryOperationType;  
    if (line[i] == '+') value = "+";
    if (line[i] == '-') value = "-";
    if (line[i] == '*') value = "*";
    if (line[i] == '/') value = "/";

    token_len = 1;
  } else if (line[i] == '=') {
    type = AssignmentOperatorType;
    value = "=";
    token_len = 1;
  } else if (line[i] == ':') {
    type = ColonAssignmentType;
    value = ":";
    token_len = 1;
  } else if (line[i] == ';') {
    type = SemiColonType;
    value = ";";
    token_len = 1;
  } else {
    if (isdigit(line[i])) {
      type = NumberType;
      char* digit = line;
      sprintf(digit, "%d", atoi(digit + i));
      token_len = strnlen(digit, size);
      value = digit;
      *counter += (token_len - 1);
    } else if (isalpha(line[i])) { 
      int word_begin = i;
      while (isalpha(line[i]) != false || line[i] == '_') i += 1; 
      size_t i_len = i - word_begin * sizeof(char) + 1;
      *identifier = realloc(*identifier, i_len); 
      if (strncpy_s(*identifier, i_len, line + word_begin, i - word_begin) == false) {
        word_counter += 1; 
        *counter = i - 1;
        token_len = strnlen(*identifier, i_len);
        value = *identifier;
        type = IdentifierType;
        if (strncmp(value, keywords[0].keyword, strlen(keywords[0].keyword)) == 0)
          type = LetType;
      }
    }   
  } 

  *counter += 1;
 
  return create_token(type, value, token_len);
}

void print_lexer(token_list_t* tokens_list) {
  token_list_t* current = tokens_list;
  int counter = 1;
  printf("TOKEN LIST!\n");
  while (current != NULL) {
    switch (current->current.type) {
      case FnType:
        printf("(FN) ");
        break;
      case LetType:
        printf("(LET) ");
        break;
      case OpenParenthesisType:
        printf("(OPEN PARENTHESIS) ");
        break;
      case ClosedParenthesisType:
        printf("(CLOSED PARENTHESIS) ");
        break;
      case AssignmentOperatorType:
        printf("(ASSIGNMENT OPERATOR) ");
        break;
      case IntType:
        printf("(INT) ");
        break;
      case ColonAssignmentType:
        printf("(:) ");
        break;
      case OpenBracketType:
        printf("(OPEN ANGULAR BRACKET) ");
        break;
      case ClosedBracketType:
        printf("(CLOSED ANGULAR BRACKET) ");
        break;
      case SemiColonType:
        printf("(;) ");
        break;
      case EndType:
        printf("(END) ");
        break;
      case CommentType:
        printf("(COMMENT) ");
        break;
      case IdentifierType:
        printf("(IDENTIFIER) ");
        break;
      case BinaryOperationType:
        printf("(BINARY OPERATION) ");
        break;
      case NumberType:
        printf("(NUMBER) ");
        break;
      default:
        continue;
    }
    printf("[%d]: length (%d): [%s]\n", counter, current->current.strlen, current->current.value);
    ++counter;
    current = current->next;
  }
}

