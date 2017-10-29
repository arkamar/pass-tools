#define RECORD_SEPARATOR 0x1e
#define UNIT_SEPARATOR 0x1f

enum {
	PASS_SUCCESS,
	PASS_ERROR
};

struct pass {
	const char * service;
	const char * username;
	const char * pass;
	const char * info;
};

int pass_parse(struct pass * restrict, char * restrict);
int pass_print_info(const struct pass *, FILE *);
int pass_print_out(const struct pass *, FILE *);
void * xmemset(void *s, int c, size_t n);
