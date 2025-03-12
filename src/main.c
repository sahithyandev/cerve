#include <stdio.h>
#include <string.h>

#include "./subcmds/help.c"
#include "./subcmds/serve.c"
#include "./utils/log_macros.h"

int LOG_LEVEL = LOG_NORMAL;

int main(int argc, char **argv) {
  if (argc == 1) {
    return subcmd_serve();
  }
  if (strcmp(argv[1], "--verbose") == 0) {
    LOG_LEVEL = LOG_VERBOSE;
    return subcmd_serve();
  }

  if (strcmp("help", argv[1]) == 0) {
    subcmd_help();
    return 0;
  }
  if (strcmp("version", argv[1]) == 0) {
    subcmd_version();
    return 0;
  }

  printf("Unknown command: %s\n", argv[1]);
  printf("Run `cerve help` for usage information\n");
  return 0;
}
