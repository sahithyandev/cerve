#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

char *normalize_url_segment(char path_segment[]) {
	char* normalized = malloc(PATH_MAX * sizeof(char));
	strcpy(normalized, path_segment);
	if (ends_with(path_segment, "/") == 1) {
		strcat(normalized, "index.html");
	}
	return normalized;
}

char *file_extension_to_mime(char file_extension[]) {
	if (strcmp(file_extension, "html") == 0) {
		return "text/html";
	}
	if (strcmp(file_extension, "css") == 0) {
		return "text/css";
	}
	if (strcmp(file_extension, "js") == 0 || strcmp(file_extension, "mjs") == 0) {
		return "text/javascript";
	}
	if (strcmp(file_extension, "json") == 0) {
		return "application/json";
	}
	if (strcmp(file_extension, "jpg") == 0 || strcmp(file_extension, "jpeg") == 0) {
		return "image/jpeg";
	}
	if (strcmp(file_extension, "ico") == 0) {
		return "image/vnd.microsoft.icon";
	}
	if (strcmp(file_extension, "png") == 0) {
		return "image/png";
	}
	return "text/plain";
}

int file_size(FILE *file_pointer) {
	struct stat file_stat;
	fstat(fileno(file_pointer), &file_stat);
	return file_stat.st_size;
}
