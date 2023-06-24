#include "sev.h"
#include <stdio.h>
#include <string.h>


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

  ast_list_t* ast_list = produce_ast(tokens_list); 
  if (ast_list == NULL) return 0;
  
  
  //char* file_name_ext = strtok(strstr(filename, "/") + 1, ".sev"); //QUICK AND DIRTY 
  FILE* transpiled_file = NULL;
  int err_num = fopen_s(&transpiled_file, "EDIT_THIS_PLEASE.c", "w");
  if (err_num == 0) {
    fprintf(transpiled_file, "#include <stdio.h>\n"); //Write stdlib files here
    write_ast_to_file(transpiled_file, ast_list);
    fclose(transpiled_file);
  } else {
    printf("ERROR WRITING FILE! ERRNUM[%d]\n", err_num);
  }
  
  free_ast_list(&ast_list);
  free_source_file(&loaded_file);
  list_free_all_tokens(&tokens_list);
  return 0;
}
