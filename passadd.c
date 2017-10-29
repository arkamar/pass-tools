#include <linux/random.h>
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
	const char * service, * user, * info = NULL;
	char * password;
	char * line = 0;
	size_t cap = 0;
	struct pass pass;
	ssize_t len;

	argv0 = *argv; argv++; argc--;

	if (argc != 2 && argc != 3)
		usage();

	service = *argv; argv++; argc--;
	user = *argv; argv++; argc--;

	if (argv)
		info = *argv; argv++; argc--;

	while (!isatty(0) && (len = getdelim(&line, &cap, RECORD_SEPARATOR, stdin)) > 0) {
		fwrite(line, 1, len, stdout);
		if (pass_parse(&pass, line)
				&& !strcmp(pass.service, service)
				&& !strcmp(pass.username, user)) {
			fprintf(stderr,
				"Record for service '%s' with user name '%s' is already here\n",
				service, user);
			return 1;
		}
		xmemset(line, 0, cap);
	}

	fputs(service, stdout);
	fputc(UNIT_SEPARATOR, stdout);
	fputs(user, stdout);
	fputc(UNIT_SEPARATOR, stdout);
	password = getpass("Password: ");
	fputs(password, stdout);
	fputc(UNIT_SEPARATOR, stdout);
	if (info)
		fputs(info, stdout);
	fputc(RECORD_SEPARATOR, stdout);

	xmemset(password, 0, strlen(password));

	fclose(stdout);
	free(line);

	return 0;
}
