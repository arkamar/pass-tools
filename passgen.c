#include <stdio.h>
#include <stdlib.h>

#include "arg.h"
#include "pass.h"

char * argv0;

void
usage() {
	fprintf(stderr, "usage: %s [-ab] [-s SIZE]\n"
		"\n"
		"   -a   letters and numbers 'A-Za-z0-9'\n"
		"   -b   like -a plus '-_'\n"
		"   -s   password size\n"
		"\n", argv0);
	exit(PASS_ERROR);
}

int
main(int argc, char * argv[]) {
	unsigned char * buf;
	size_t len, size = 33;
	enum pass_tr pt = PASS_TR_PRINT;

	ARGBEGIN {
	case 'a':
		pt = PASS_TR_ALNUM;
		break;
	case 'b':
		pt = PASS_TR_ALNUM_HAS_UNDER;
		break;
	case 's':
		size = strtol(EARGF(usage()), NULL, 0) + 1;
		break;
	default:
		usage();
	} ARGEND

	buf = malloc(size);
	if (!buf) {
		fputs("Cannot alocate memory\n", stderr);
		return PASS_ERROR;
	}
	len = pass_getrandom(buf, size);
	buf[--len] = '\0';
	pass_tr(buf, len, pt);
	fwrite(buf, 1, len, stdout);
	xmemset(buf, 0, len);

	if (buf)
		free(buf);

	return PASS_SUCCESS;
}
