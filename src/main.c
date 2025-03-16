#include <stdio.h>
#include <string.h>

#include "./subcmds/help.c"
#include "./subcmds/serve.c"
#include "./utils/log_macros.h"

int LOG_LEVEL = LOG_NORMAL;

int main(const int argc, char **argv) {
  if (argc > 1) {
    if (strcmp("help", argv[1]) == 0) {
      subcmd_help();
      return 0;
    }
    if (strcmp("version", argv[1]) == 0) {
      subcmd_version();
      return 0;
    }
  }

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--verbose") == 0) {
      LOG_LEVEL = LOG_VERBOSE;
    }
  }

  subcmd_serve();
  return 0;
}
