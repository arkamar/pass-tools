#include <linux/random.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pass.h"

static const
char * argv0;

static
void
usage() {
	fprintf(stderr, "usage: %s file service username [info]\n", argv0);
	exit(1);
}

int
main(int argc, char * argv[]) {
	const char * filename, * service, * user, * info = NULL;
	char * line = 0;
	size_t cap = 0;
	struct pass pass;
	ssize_t len;
	FILE * f;

	argv0 = *argv; argv++; argc--;

	if (argc != 3 && argc != 4)
		usage();

	filename = *argv; argv++; argc--;
	service = *argv; argv++; argc--;
	user = *argv; argv++; argc--;
	if (argv)
		info = *argv; argv++; argc--;

	if (!(f = fopen(filename, "r+"))) {
		perror("Cannot open file");
		return 1;
	}

	while (getdelim(&line, &cap, RECORD_SEPARATOR, f) > 0) {
		if (pass_parse(&pass, line)
				&& !strcmp(pass.service, service)
				&& !strcmp(pass.username, user)) {
			fprintf(stderr,
				"Record for service '%s' with user name '%s' is already here\n",
				service, user);
			return 1;
		}
		memset(line, 0, cap);
	}

	fputs(service, f);
	fputc(UNIT_SEPARATOR, f);
	fputs(user, f);
	fputc(UNIT_SEPARATOR, f);
	while ((len = fread(line, 1, cap, stdin)) > 0) {
		fwrite(line, 1, len, f);
	}
	fputc(UNIT_SEPARATOR, f);
	if (info)
		fputs(info, f);
	fputc(RECORD_SEPARATOR, f);

	fclose(f);
	free(line);

	return 0;
}
