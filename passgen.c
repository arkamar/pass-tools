#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arg.h"
#include "pass.h"

enum pass_tr {
	PASS_TR_PRINT,
	PASS_TR_ALNUM,
	PASS_TR_ALNUM_HAS_UNDER,
};

char * argv0;

void
usage() {
	fprintf(stderr, "usage: %s [-ab]\n", argv0);
	exit(PASS_ERROR);
}

int
pass_getrandom(void * in, size_t size) {
	char * buf = in;
	int fd = open("/dev/urandom", O_RDONLY);
	size_t len;
	if (fd == -1)
		return PASS_ERROR;
	len = read(fd, buf, size);
	buf[len] = '\0';
	close(fd);
	return len;
}

static
unsigned char
tr_all_printable(unsigned char c) {
	return c % ('~' - ' ') + ' ';
}

static
unsigned char
tr_alphanum(unsigned char c) {
	c %= 2 * ('z' - 'a') + '9' - '0' + 3;
	if (c < 'z' - 'a' + 1)
		return c + 'a';
	c -= 'z' - 'a' + 1;
	if (c < 'Z' - 'A' + 1)
		return c + 'A';
	c -= 'Z' - 'A' + 1;
	return c + '0';
}

static
unsigned char
tr_alphanum_hyp_uder(unsigned char c) {
	c %= 2 * ('z' - 'a') + '9' - '0' + 5;
	if (c < 2 * ('z' - 'a') + '9' - '0' + 3)
		return tr_alphanum(c);
	if (c % 2)
		return '-';
	else
		return '_';
}

unsigned char (*callback[])(unsigned char) = {
	[PASS_TR_PRINT] = tr_all_printable,
	[PASS_TR_ALNUM] = tr_alphanum,
	[PASS_TR_ALNUM_HAS_UNDER] = tr_alphanum_hyp_uder
};

int
pass_tr(unsigned char * pass, const size_t len,
		unsigned char (*tr_fun)(unsigned char)) {
	int i;
	for (i = 0; i < len; i++)
		pass[i] = tr_fun(pass[i]);

	return PASS_SUCCESS;
}

int
main(int argc, char * argv[]) {
	unsigned char buf[256];
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
	pass_tr(buf, len, callback[pt]);
	fwrite(buf, 1, len, stdout);
	return PASS_SUCCESS;
}
