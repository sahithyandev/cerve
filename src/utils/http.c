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