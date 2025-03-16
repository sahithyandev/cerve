#include "./version.c"

void subcmd_help() {
  subcmd_version();

  puts("Serve with C\n");
  puts("When run without any commands, a simple HTTP server will be started.");
  puts("Commands");
  puts("    help    - prints this message");
  puts("    version - prints the current version");
}

