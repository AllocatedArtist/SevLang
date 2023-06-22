#include "sev.h"

const char* init_sev(int argc, char** argv) {
  char* context = NULL;
  char* command = NULL;
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

  return filename;
}
