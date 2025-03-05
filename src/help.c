#include "config.h"
#include "./version.c"

void subcmd_help() {
  subcmd_version();

  printf("Commands\n");
  printf("    help    - prints this message\n");
  printf("    version - prints the current version\n");
}

