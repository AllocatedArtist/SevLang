#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

token_t tokenize(char* line, size_t size, size_t* counter) {
  enum TokenType type = NoneType;
  const char* value = NULL;
  size_t strlen = 0;
  int i = *counter;
  
  if (line[i] == '(') {
    type = OpenParenthesisType;
    value = "(";
    strlen = 1;
  } else if (line[i] == ')') {
    type = ClosedParenthesisType;
    value = ")";
    strlen = 1;
  } else if (line[i] == '+' || 
            line[i] == '-'  || 
            line[i] == '*'  || 
            line[i] == '/') {

    type = BinaryOperationType;  
    if (line[i] == '+') value = "+";
    if (line[i] == '-') value = "-";
    if (line[i] == '*') value = "*";
    if (line[i] == '/') value = "/";

    strlen = 1;
  } else if (line[i] == '=') {
    type = AssignmentOperatorType;
    value = "=";
    strlen = 1;
  } else if (line[i] == ':') {
    type = ColonAssignmentType;
    value = ":";
    strlen = 1;
  } else {
    if (isdigit(line[i])) {
      type = NumberType;
      char* digit = line;
      sprintf(digit, "%d", atoi(digit + i));
      strlen = strnlen(digit, size);
      value = digit;
      *counter += (strlen - 1);
    } else if (isalpha(line[i])) { 
     
    }
  }

  *counter += 1;
 
  return create_token(type, value, strlen);
}

int main(int argc, char** argv) {
  char* command = NULL;
  char* context = NULL;
  const char* filename = NULL;

  for (int i = 0; i < argc; ++i) {
    if (!strcmp(argv[i], "sev") || !strcmp(argv[i], "sev.exe")) {
      if (i != 0) {
        printf("Don't use 'sev' in the middle of commands please!\n");
        break;
      }
      if (argc == 1) printf("Use sev --h for help\n");
      continue;
    }

    context = argv[i];

    command = strstr(argv[i], "--");

    if (command == NULL) {
      printf("No command found.\n");
      printf("Use sev --h for help\n");
      break;
    } else {
      command = strtok_s(command, "--", &context);
      if (strcmp(command, "t") == 0) {
        if (argc == 3) {
          filename = argv[i + 1];  
        } else if (argc < 3) {
          printf("Need argument after command.\n");
          break;
        } else {
          printf("Too many arguments!\n");
          break;
        }
      } else if (strcmp(command, "h") == 0) {
        printf("sev --[COMMAND] argument\n");
        printf("-t Transpile code to C. Argument is a filename you wish to transpile.\n");
        printf("-h Well you just used this.\n");
      } else {
        printf("Invalid command!\n");
        break;
      }
    }

    if (i == 1) break;
  }

  char* file_contents = NULL;
  size_t file_size = 0;
  if (filename != NULL) {
    FILE* file = NULL;
    if (fopen_s(&file, filename, "rb") != 0) {
      printf("File [%s] does not exist!", filename);
      return -1;
    }
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    file_contents = (char*)malloc(sizeof(char) * file_size);
    if (file_contents == NULL) {
      printf("Unable to allocate memory for size of file!\n");
      fclose(file);
      return -1;
    }
    size_t result = fread(file_contents, 1, file_size, file); 
    if (result != file_size) {
      printf("Error reading file %s!\n", filename);
      return -1;
    }
    file_contents[file_size] = '\0';
    
    fclose(file);
  } else {
    printf("Nothing happened.\n");
  }

  token_list_t* tokens_list = NULL;

  if (file_contents != NULL) {
    size_t number_of_whitespaces = 0;
    int i = 0;
    while (i < file_size - 1) {
      if (isspace(file_contents[i])) ++number_of_whitespaces;
      ++i;
    }
    size_t size = file_size - number_of_whitespaces;
    char* trimmed_file = (char*)malloc(size);
    if (trimmed_file == NULL) {
      printf("Unable to allocate memory for splitting whitespace!\n");
      return -1;
    }
    remove_whitespace(trimmed_file, file_contents);
    size_t counter = 0;

    while (counter != size - 1) {
      token_t token = tokenize(trimmed_file, size - 1, &counter); 
      if (token.type != NoneType && token.value != NULL) list_add_token(&tokens_list, token);  
    }
    
    free(trimmed_file);
    free(file_contents);
  }  
  
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

  list_free_all_tokens(&tokens_list);

  return 0;
}
