#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <fcntl.h>


const char* SELF_EXE = "/proc/self/exe";
const char* ENV_NAME = "STFU_EXE";

// In large parts inspired by https://stackoverflow.com/a/24582217/2058753
char * getSelfExecutableName() {
	size_t linkname_size = 1024;
	ssize_t r;
	char * linkname;

	// Try reading the process name into a buffer of increasing size
	while(1) {
		linkname = malloc(linkname_size + 1);
		if (linkname == NULL) {
			fprintf(stderr, "Insufficient memory\n");
			exit(EXIT_FAILURE);
		}

		r = readlink(SELF_EXE, linkname, linkname_size);

		if(r < 0) {
			perror("readlink");
			exit(EXIT_FAILURE);
		}

		if (r < linkname_size) {
			break;
		}
		free(linkname);
		linkname_size *= 2;
	}

	char *tmp;

	tmp = realloc(linkname, r + 1);
	if (tmp) {
		linkname = tmp;
		linkname_size = r + 1;
	}

	linkname[r] = '\0';
	return linkname;
}

void suppress() {
	// Open /dev/null
	int devNull = open("/dev/null", O_RDWR);
	
	// Redirect stdin, stdout and stderr to /dev/null
	for (int i = 0; i < 3; ++i) {
		dup2(devNull, i);
	}
}

__attribute__((constructor)) static void initialize() {
	 // Get the name of the executable that is starting
	char * linkname = getSelfExecutableName();
	char * executableName = basename(linkname);

	// Get the name of the executable we want to silence
	char * suppressed = getenv(ENV_NAME);
	
	if(strcmp(executableName, suppressed) == 0) {
		suppress();
	} else {
		char * suppressedListBuf = malloc(strlen(suppressed) + 1);
		strcpy(suppressedListBuf, suppressed);
		char * token = strtok(suppressedListBuf, ":");
		if(strcmp(executableName, token) == 0) {
			suppress();
		}
		while(token = strtok(NULL, ":")) {
			if(strcmp(executableName, token) == 0) {
				suppress();
			}
		}
	}

	free(linkname);
}
