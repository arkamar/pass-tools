#include <stdio.h>
#include <string.h>

#include "pass.h"

int
pass_parse(struct pass * restrict pass, char * restrict line) {
	const char ** arr = (void *)pass;
	char * s;
	int i;
	memset(pass, 0, sizeof *pass);
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

void *
xmemset(void * s, int c, size_t n) {
	size_t i;
	char * ss = s;
	for (i = 0; i < n; i++)
		ss[i] = c;
	return ss;
}
