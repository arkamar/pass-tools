#define RECORD_SEPARATOR 0x1e
#define UNIT_SEPARATOR 0x1f

enum {
	PASS_SUCCESS,
	PASS_ERROR
};

enum pass_tr {
	PASS_TR_PRINT,
	PASS_TR_ALNUM,
	PASS_TR_ALNUM_HAS_UNDER,
	PASS_TR_LENGTH
};

struct pass {
	const char * service;
	const char * username;
	const char * pass;
	const char * info;
};

int pass_getrandom(void *, const size_t);
int pass_tr(unsigned char *, const size_t, const enum pass_tr);

int pass_parse(struct pass * restrict, char * restrict);
int pass_print_info(const struct pass *, FILE *);
int pass_print_out(const struct pass *, FILE *);
void * xmemset(void *s, int c, size_t n);
