#include "./version.c"

void subcmd_help() {
  subcmd_version();

  puts("Serve with C\n\n");
  puts("When run without any commands, a simple HTTP server will be started.\n");
  puts("Commands\n");
  puts("    help    - prints this message\n");
  puts("    version - prints the current version\n");
}

