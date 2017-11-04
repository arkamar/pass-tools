#include <stdio.h>
#include <stdlib.h>

#include "arg.h"
#include "pass.h"

char * argv0;

void
usage() {
	fprintf(stderr, "usage: %s [-ab]\n"
		"\n"
		"   -a   letters and numbers 'A-Za-z0-9'\n"
		"   -b   like -a plus '-_'\n"
		"\n", argv0);
	exit(PASS_ERROR);
}

int
main(int argc, char * argv[]) {
	unsigned char buf[32];
	size_t len = sizeof buf;
	int i;
	enum pass_tr pt = PASS_TR_PRINT;

	ARGBEGIN {
	case 'a':
		pt = PASS_TR_ALNUM;
		break;
	case 'b':
		pt = PASS_TR_ALNUM_HAS_UNDER;
		break;
	default:
		usage();
	} ARGEND

	for (i = 0; i < len; i++)
		buf[i] = i;
	len = pass_getrandom(buf, sizeof buf);
	pass_tr(buf, len, pt);
	fwrite(buf, 1, len, stdout);
	xmemset(buf, 0, len);
	return PASS_SUCCESS;
}
