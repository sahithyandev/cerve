#include "./version.c"

void subcmd_help() {
	subcmd_version();

	puts("Serve with C\n\n"
		 "USAGE\n"
		 "   $ cerve help            - Prints this message\n"
		 "   $ cerve version         - Prints current version\n"
		 "   $ cerve folder_name     - Starts a HTTP server serving "
		 "folder_name \n");
}
