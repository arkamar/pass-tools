#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arg.h"
#include "pass.h"

char * argv0;

static
void
usage() {
	fprintf(stderr, "usage: %s [-abp] [-s SIZE] service username [info]\n"
		"\n"
		"   -a   letters and numbers 'A-Za-z0-9'\n"
		"   -b   like -a plus '-_'\n"
		"   -p   password will be added by user\n"
		"   -s   password size\n"
		"\n", argv0);
	exit(1);
}

int
main(int argc, char * argv[]) {
	char * line = 0;
	unsigned char * buf = NULL;
	size_t cap = 0;
	struct pass pass, new;
	ssize_t len, size = 33;
	int ask_for_password = 0;
	enum pass_tr tr = PASS_TR_PRINT;

	ARGBEGIN {
	case 'a':
		tr = PASS_TR_ALNUM;
		break;
	case 'b':
		tr = PASS_TR_ALNUM_HAS_UNDER;
		break;
	case 'p':
		ask_for_password = 1;
		break;
	case 's':
		size = strtol(EARGF(usage()), NULL, 0) + 1;
		break;
	} ARGEND

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

	if (ask_for_password) {
		new.pass = getpass("Password: ");
	} else {
		buf = malloc(size);
		if (!buf) {
			fputs("Cannot allocate memory\n", stderr);
			return PASS_ERROR;
		}
		len = pass_getrandom(buf, size);
		buf[--len] = '\0';
		pass_tr(buf, len, tr);
		new.pass = (char*)buf;
	}
	pass_print_out(&new, stdout);
	xmemset((void*)new.pass, 0, strlen(new.pass));

	fclose(stdout);
	free(line);
	if (buf)
		free(buf);

	return PASS_SUCCESS;
}
