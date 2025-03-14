char* status_code_to_str(int status_code) {
  switch (status_code) {
    case 200:
      return "OK";
    case 404:
      return "Not Found";
    case 413:
      return "Request Entity Too Large";
    default:
      return "Unknown";
  }
}

void create_response(
    char* response_buffer,
    signed short int status_code,
    const char* message) {

  if (message == NULL) {
    message = status_code_to_str(status_code);
  }

sprintf(response_buffer,
  "HTTP/1.1 %hi %s\r\n"
  "Content-Type: text/plain\r\n"
  "Content-length: %lu\r\n"
  "\r\n"
  "%s", status_code, status_code_to_str(status_code), strlen(message), message);

}