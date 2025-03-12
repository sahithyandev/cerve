#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

char* normalize_url_segment(char path_segment[]) {
  if (strcmp(path_segment, "/") == 0) {
    return "/index.html";
  }
  return path_segment;
}

char* convert_url_segment_to_file_location(char path_segment[]) {
  printf(path_segment);
  char* cwd = (char*) malloc(PATH_MAX * sizeof(char));
  if (getcwd(cwd, PATH_MAX) == NULL) {
    perror("getcwd() error");
    exit(1);
  }
  printf("CWD: %s\n", cwd);
  char *normalized = normalize_url_segment(path_segment);
  printf("normalized: %s\n", normalized);
  strcat(cwd, normalized);
  printf("CWD after strcat: %s\n", cwd);
  return cwd;
}
