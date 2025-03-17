#include "./version.c"

void subcmd_help() {
	subcmd_version();

	puts("Serve with C\n\n"
		 "USAGE\n"
		 "   $ cerve help				- Prints this message\n"
		 "   $ cerve version			- Prints current version\n"
		 "   $ cerve [--listen PORT] folder_name	- Starts a HTTP server serving\n\n"
		 "help and version commands don't take in any options.\n\n"
		 "OPTIONS FOR SERVING\n"
		 "   -l PORT, --listen PORT		 - Listens on PORT. Default 2000. Must be bigger than 1024.");
}
