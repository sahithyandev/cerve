#include <stdio.h>
#include <string.h>

#include "./help.c"
#include "./serve.c"

int main(int argc, char **argv) {
  if (argc == 1) {
    return subcmd_serve();
  }
  else if (strcmp("help", argv[1]) == 0) {
    subcmd_help();
    return 0;
  }
  else if (strcmp("version", argv[1]) == 0) {
    subcmd_version();
    return 0;
  }

  printf("Unknown command: %s\n", argv[1]);
  printf("Run `cerve help` for usage information\n");
  return 0;
}
