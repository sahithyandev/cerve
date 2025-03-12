#include "../config.h"

#include "./version.c"

void subcmd_help() {
  subcmd_version();

  printf("Serve with C\n\n");
  printf("When run without any commands, a simple HTTP server will be started.\n");
  printf("Commands\n");
  printf("    help    - prints this message\n");
  printf("    version - prints the current version\n");
}

