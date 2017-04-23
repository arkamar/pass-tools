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
	return i == 4;
}

void
pass_print(const struct pass * pass) {
	fprintf(stderr, "account: %s\n", pass->account);
	fprintf(stderr, "user:    %s\n", pass->username);
	fprintf(stderr, "pass:    %s\n", pass->pass);
	fprintf(stderr, "addi:    %s\n", pass->additnial);
}

