#include <stdio.h>
#include <stdlib.h>

#include "pass.h"

static
char * argv0;

static
void
usage() {
	fprintf(stderr, "usage: %s\n", argv0);
	exit(1);
}

int
main(int argc, char * argv[]) {
	char * line = NULL;
	size_t cap = 0;
	struct pass pass;

	argv0 = *argv; argv++; argc--;

	if (argc)
		usage();

	while (getdelim(&line, &cap, RECORD_SEPARATOR, stdin) > 0)
		if (pass_parse(&pass, line))
			pass_print_info(&pass, stdout);

	free(line);

	return 0;
}
