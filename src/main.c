#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include <cwalk.h>

#include "./subcmds/help.c"
#include "./subcmds/serve.c"

char SERVING_DIR[PATH_MAX];

int main(const int argc, char **argv) {
	int option;
	struct option long_options[] = {{"listen", required_argument, 0, 'l'}, {0, 0, 0, 0}};

	int port = 2000;
	while ((option = getopt_long(argc, argv, "l:v", long_options, NULL)) != -1) {
		switch (option) {
		case 'l':
			port = atoi(optarg);
			if (port == 0 || port < 1024) {
				port = 2000;
			}
			break;
		default:
			fprintf(stderr, "Unknown option.\n");
			return 1;
		}
	}

	char *command_name = "./";
	if (optind < argc) {
		command_name = argv[optind];
	}

	if (strcmp("help", command_name) == 0) {
		subcmd_help();
		return 0;
	}
	if (strcmp("version", command_name) == 0) {
		subcmd_version();
		return 0;
	}

	if (getcwd(SERVING_DIR, PATH_MAX) == NULL) {
		perror("getcwd() error");
		exit(1);
	}
	cwk_path_join(SERVING_DIR, command_name, SERVING_DIR, sizeof(SERVING_DIR));
	subcmd_serve(port);
	return 0;
}
