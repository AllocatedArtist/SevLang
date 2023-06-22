#include "sev.h"

source_file_t load_source_file(const char* filename) {
  char* file_contents = NULL;
  size_t file_size = 0;
  if (filename != NULL) {
    FILE* file = NULL;
    if (fopen_s(&file, filename, "rb") != 0) {
      printf("File [%s] does not exist!", filename);
      return (source_file_t) { NULL, 0 };
    }
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    file_contents = (char*)malloc(sizeof(char) * file_size);
    if (file_contents == NULL) {
      printf("Unable to allocate memory for size of file!\n");
      fclose(file);
      return (source_file_t) { NULL, 0 };
    }
    size_t result = fread(file_contents, 1, file_size, file); 
    if (result != file_size) {
      printf("Error reading file %s!\n", filename);
      return (source_file_t) { NULL, 0 };
    }
    file_contents[file_size] = '\0';
    
    fclose(file);
  } else {
    printf("Nothing happened.\n");
  } 
  
  return (source_file_t) { file_contents, file_size };
}

void free_source_file(source_file_t* source_file) {
  free(source_file->file_contents);
}

