#include "sev.h"

int main(int argc, char** argv) {  
  const char* filename = init_sev(argc, argv); 

  if (filename == NULL) return 0;

  source_file_t loaded_file = load_source_file(filename);

  if (loaded_file.file_contents == NULL || loaded_file.file_contents == 0) return 0;
  token_list_t* tokens_list = NULL;

  size_t counter = 0;
  char* identifier = (char*)malloc(loaded_file.file_size);

  if (identifier == NULL) {
    printf("Could not initially allocate memory for token identifier!\n");
    exit(-1);
  }

  while (counter != loaded_file.file_size - 1) {
    token_t token = tokenize(loaded_file.file_contents, loaded_file.file_size - 1, &counter, &identifier); 
    if (token.type != NoneType && token.value != NULL) list_add_token(&tokens_list, token);  
  }
  free(identifier);
  free_source_file(&loaded_file);

  print_lexer(tokens_list);
  
  list_free_all_tokens(&tokens_list);

  return 0;
}
