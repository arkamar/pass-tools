#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "pass.h"

int
pass_getrandom(void * in, const size_t size) {
	char * buf = in;
	int fd = open("/dev/urandom", O_RDONLY);
	size_t len;
	if (fd == -1)
		return PASS_ERROR;
	len = read(fd, buf, size);
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

static
unsigned char
call_tr(const enum pass_tr tr, unsigned char c) {
	switch (tr) {
	case PASS_TR_ALNUM:
		return tr_alphanum(c);
	case PASS_TR_ALNUM_HAS_UNDER:
		return tr_alphanum_hyp_uder(c);
	default:
		fputs("Unknown transformation\n", stderr);
	case PASS_TR_PRINT:
		return tr_all_printable(c);
	}
}

int
pass_tr(unsigned char * pass, const size_t len, const enum pass_tr tr) {
	int i;

	if (tr >= PASS_TR_LENGTH)
		return PASS_ERROR;

	for (i = 0; i < len; i++)
		pass[i] = call_tr(tr, pass[i]);

	return PASS_SUCCESS;
}

int
pass_parse(struct pass * restrict pass, char * restrict line) {
	const char ** arr = (void *)pass;
	char * s;
	int i;
	xmemset(pass, 0, sizeof *pass);
	for (s = line, i = 0; *s; s++) {
		if (i < (sizeof * pass / sizeof(char *)) && !arr[i])
			arr[i] = s;
		if (*s == UNIT_SEPARATOR || *s == RECORD_SEPARATOR) {
			*s = '\0';
			i++;
			continue;
		}
	}
	return i == 3 || i == 4 ? PASS_SUCCESS : PASS_ERROR;
}

int
pass_print_info(const struct pass * pass, FILE * f) {
	fputs(pass->service, f);
	fputc('\t', f);
	fputs(pass->username, f);
	if (pass->info) {
		fputc('\t', f);
		fputs(pass->info, f);
	}
	fputc('\n', f);
	return PASS_SUCCESS;
}

int
pass_print_out(const struct pass * restrict pass, FILE * restrict f) {
	fputs(pass->service, f);
	fputc(UNIT_SEPARATOR, f);
	fputs(pass->username, f);
	fputc(UNIT_SEPARATOR, f);
	fputs(pass->pass, f);
	fputc(UNIT_SEPARATOR, f);
	if (pass->info)
		fputs(pass->info, f);
	fputc(RECORD_SEPARATOR, f);
	return PASS_SUCCESS;
}

int
pass_clear(struct pass * pass) {
	if (pass->service)
		xmemset(pass->service, 0, strlen(pass->service));
	if (pass->username)
		xmemset(pass->username, 0, strlen(pass->username));
	if (pass->pass)
		xmemset(pass->pass, 0, strlen(pass->pass));
	if (pass->info)
		xmemset(pass->info, 0, strlen(pass->info));
	return PASS_SUCCESS;
}

void *
xmemset(void * s, int c, size_t n) {
	size_t i;
	char * ss = s;
	for (i = 0; i < n; i++)
		ss[i] = c;
	return ss;
}
