typedef struct {
        size_t size;
        size_t used;
        char* data;
} strbld_t;

int sb_init(strbld_t*);

void sb_free(strbld_t*);

int sb_append(strbld_t*, char);