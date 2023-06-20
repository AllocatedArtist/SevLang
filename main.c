#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  if (file_contents != NULL) {
    char* ctx = file_contents;
    char* line = strtok_s(file_contents, "\n", &ctx);
    while (line != NULL) {
      printf("LINE: %s\n", line); 
      line = strtok_s(NULL, "\n", &ctx);
    }
    free(file_contents);
  }

  return 0;
}
