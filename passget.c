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
	fprintf(stderr, "usage: %s pattern\n", argv0);
	exit(1);
}

int
main(int argc, char * argv[]) {
	char * patt, * line = NULL;
	size_t cap = 0;
	ssize_t len, found = 0;
	struct pass pass;

	argv0 = *argv; argv++; argc--;

	if (argc != 1)
		usage();

	patt = *argv; argv++; argc--;

	if (isatty(fileno(stdout))) {
		fprintf(stderr, "%s does not print passwords to ttys, "
			"redirect output to somewhre else\n"
			"\n"
			"For example: %s %s | xclip\n", argv0, argv0, patt);
		return 1;
	}

	while ((len = getdelim(&line, &cap, RECORD_SEPARATOR, stdin)) > 0) {
		if (pass_parse(&pass, line) && strstr(pass.account, patt)) {
			found++;
			fprintf(stderr, "account: %s\n", pass.account);
			fputs(pass.pass, stdout);
		}
		memset(line, 0, cap);
	}

	if (found == 0)
		fprintf(stderr, "No record found for patter '%s'\n", patt);
	if (found != 1)
		fprintf(stderr, "Multiple records (%lu) found for patter '%s'\n", found, patt);

	return found != 1;
}
