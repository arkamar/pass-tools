#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pass.h"

static
char * argv0;

static
void
usage() {
	fprintf(stderr, "usage: %s service [username]\n", argv0);
	exit(1);
}

int
main(int argc, char * argv[]) {
	char * patt, * user = NULL, * line = NULL;
	size_t cap = 0;
	ssize_t len, found = 0;
	struct pass pass;

	argv0 = *argv; argv++; argc--;

	if (argc != 1 && argc != 2)
		usage();

	patt = *argv; argv++; argc--;
	if (argv && *argv)
		user = *argv; argv++; argc--;

	if (isatty(fileno(stdout))) {
		fprintf(stderr, "%s does not print pass phrases to ttys, "
			"redirect output to somewhere else\n"
			"\n"
			"For example: %s %s | xclip\n", argv0, argv0, patt);
		return 1;
	}

	while ((len = getdelim(&line, &cap, RECORD_SEPARATOR, stdin)) > 0) {
		if (pass_parse(&pass, line) == PASS_SUCCESS && strstr(pass.service, patt)
				&& (!user ? 1 : !!strstr(pass.username, user))) {
			found++;
			pass_print_info(&pass, stderr);
			if (found == 1)
				fputs(pass.pass, stdout);
		}
		memset(line, 0, cap);
	}

	free(line);

	if (found == 0)
		fprintf(stderr, "No record found for service pattern '%s'", patt);
	if (found > 1)
		fprintf(stderr, "Multiple records (%lu) found for service pattern '%s'", found, patt);
	if (found == 0 || found > 1) {
		if (user)
			fprintf(stderr, " and user pattern '%s'\n", user);
		else
			fputc('\n', stderr);
	}

	return found != 1;
}
