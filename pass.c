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
	return i == 3 || i == 4;
}

void
pass_print_info(const struct pass * pass, FILE * f) {
	fputs(pass->service, f);
	fputc('\t', f);
	fputs(pass->username, f);
	if (pass->more) {
		fputc('\t', f);
		fputs(pass->more, f);
	}
	fputc('\n', f);
}

void *
xmemset(void * s, int c, size_t n) {
	size_t i;
	char * ss = s;
	for (i = 0; i < n; i++)
		ss[i] = c;
	return ss;
}
