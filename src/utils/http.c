#include "dirent.h"

#include "../config.h"

char *status_code_to_str(const int status_code) {
	switch (status_code) {
	case 200:
		return "OK";
	case 302:
		return "Found";
	case 404:
		return "Not Found";
	case 413:
		return "Request Entity Too Large";
	default:
		return "Unknown";
	}
}

void create_response(char *response_buffer, const signed short int status_code,
					 const char *message) {

	if (message == NULL) {
		message = status_code_to_str(status_code);
	}

	sprintf(response_buffer,
			"HTTP/1.1 %hi %s\r\n"
			"Content-Type: text/plain\r\n"
			"Content-length: %lu\r\n"
			"\r\n"
			"%s",
			status_code, status_code_to_str(status_code), strlen(message), message);
}

void create_file_response_headers(char *response_buffer, char *file_extension,
								  const int file_size) {
	sprintf(response_buffer,
			"HTTP/1.1 200 %s\r\n"
			"Content-Type: %s\r\n"
			"Content-Length: %d\r\n"
			"\r\n",
			status_code_to_str(200), file_extension_to_mime(file_extension), file_size);
}

void create_index_page_response(char *response_buffer, const char* file_path) {
	DIR *directory = opendir(file_path);
	if (directory == NULL) {
		return;
	}
	struct dirent *entity;

	char body[RESPONSE_BUFFER_SIZE];
	int offset = sprintf(body, "<html><head><title>Index of %s</title></head><body><h1>Index of %s</h1><ul>", file_path, file_path);
	while ((entity = readdir(directory)) != NULL) {
		if (strncmp(entity->d_name, ".", 1) == 0) {
			continue;
		}
		char *href = malloc(sizeof(char) * (strlen(entity->d_name) + 2));
		strcpy(href, entity->d_name);

		if (entity->d_type == DT_DIR) {
			strcat(href, "/");
		} else if (entity->d_type!=DT_REG) {
			continue;
		}
		offset += sprintf(body + offset, "<li><a href='%s'>%s</a></li>", href, entity->d_name);
		free(href);
	}
	closedir(directory);
	offset += sprintf(body + offset, "</ul></body></html>");

	sprintf(response_buffer,
			"HTTP/1.1 200 %s\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: %d\r\n"
			"\r\n%s",
			status_code_to_str(200), offset, body);
}

void create_redirect_response(char * response_buffer, const char* to, signed short int status_code) {
	sprintf(response_buffer,
			"HTTP/1.1 %d %s\r\n"
			"Location: %s\r\n"
			"Content-Length: 0\r\n\r\n\r\n",
			status_code,
			status_code_to_str(status_code), to);

}