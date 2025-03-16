#include <stdio.h>
#include <string.h>
#include <linux/limits.h>

#include <cwalk.h>

#include "./subcmds/help.c"
#include "./subcmds/serve.c"
#include "./utils/log_macros.h"

int LOG_LEVEL = LOG_NORMAL;
char SERVING_DIR[PATH_MAX];

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

  char* serve_dir_relative = "./";
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--verbose") == 0) {
      LOG_LEVEL = LOG_VERBOSE;
    } else {
      serve_dir_relative = argv[i];
      break;
    }
  }
  if (getcwd(SERVING_DIR, PATH_MAX) == NULL) {
    perror("getcwd() error");
    exit(1);
  }
  cwk_path_join(SERVING_DIR, serve_dir_relative, SERVING_DIR, sizeof(SERVING_DIR));
  subcmd_serve();
  return 0;
}
