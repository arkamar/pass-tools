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
pass_print(const struct pass * pass) {
	fputs(pass->service, stderr);
	fputc('\t', stderr);
	fputs(pass->username, stderr);
	if (pass->more) {
		fputc('\t', stderr);
		fputs(pass->more, stderr);
	}
	fputc('\n', stderr);
}

