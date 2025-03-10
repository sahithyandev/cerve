#include <stdio.h>
#include <string.h>

#include "./help.c"
#include "./serve.c"

int main(int argc, char **argv) {
  if (argc == 1) {
    start_server();
    return 0;
  }
  if (strcmp("help", argv[1]) == 0) {
    subcmd_help();
    return 0;
  }

  if (strcmp("version", argv[1]) == 0) {
    subcmd_version();
    return 0;
  }

  return 0;
}
