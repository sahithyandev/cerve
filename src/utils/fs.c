#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>

char* normalize_url_segment(char path_segment[]) {
  if (strcmp(path_segment, "/") == 0) {
    return "/index.html";
  }
  return path_segment;
}

char* convert_url_segment_to_file_location(char path_segment[]) {
  char* cwd = (char*) malloc(PATH_MAX * sizeof(char));
  if (getcwd(cwd, PATH_MAX) == NULL) {
    perror("getcwd() error");
    exit(1);
  }
  char *normalized = normalize_url_segment(path_segment);
  strcat(cwd, normalized);
  return cwd;
}

char* file_extension_to_mime(char file_extension[]) {
  if (file_extension == "html") {
    return "text/html";
  }

  return "text/plain";
}

int file_size(FILE* file_pointer) {
  struct stat file_stat;
  fstat(fileno(file_pointer), &file_stat);
  return file_stat.st_size;
}