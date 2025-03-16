#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

char* normalize_url_segment(char path_segment[]) {
  if (strcmp(path_segment, "/") == 0) {
    return "/index.html";
  }
  return path_segment;
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
