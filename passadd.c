#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pass.h"

static const
char * argv0;

static
void
usage() {
	fprintf(stderr, "usage: %s service username [info]\n", argv0);
	exit(1);
}

int
main(int argc, char * argv[]) {
	char * line = 0;
	size_t cap = 0;
	struct pass pass, new;
	ssize_t len;

	argv0 = *argv; argv++; argc--;

	if (argc != 2 && argc != 3)
		usage();

	new.service = *argv; argv++; argc--;
	new.username = *argv; argv++; argc--;

	if (argv) {
		new.info = *argv; argv++; argc--;
	} else {
		new.info = NULL;
	}

	if (isatty(fileno(stdout))) {
		fprintf(stderr, "%s does not print pass phrases to ttys, "
			"redirect output to somewhere else\n"
			"\n"
			"For example: %s %s %s | gpg ....\n", argv0, argv0, new.service, new.username);
		return 1;
	}

	while (!isatty(fileno(stdin)) && (len = getdelim(&line, &cap, RECORD_SEPARATOR, stdin)) > 0) {
		fwrite(line, 1, len, stdout);
		if (pass_parse(&pass, line) == PASS_SUCCESS
				&& !strcmp(pass.service, new.service)
				&& !strcmp(pass.username, new.username)) {
			fprintf(stderr,
				"Record for service '%s' with user name '%s' is already here\n",
				new.service, new.username);
			return PASS_ERROR;
		}
		xmemset(line, 0, cap);
	}

	new.pass = getpass("Password: ");
	pass_print_out(&new, stdout);
	xmemset((void*)new.pass, 0, strlen(new.pass));

	fclose(stdout);
	free(line);

	return PASS_SUCCESS;
}
