#define RECORD_SEPARATOR 0x1e
#define UNIT_SEPARATOR 0x1f

struct pass {
	const char * service;
	const char * username;
	const char * pass;
	const char * more;
};

int pass_parse(struct pass * restrict, char * restrict);
void pass_print(const struct pass *);
