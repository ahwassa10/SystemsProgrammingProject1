#include <stdlib.h>
#include "strbld.h"
#define BUFSIZE 10

int sb_init(strbld_t* buf) {
        buf -> size = BUFSIZE;
        
        buf -> data = malloc(sizeof(char) * BUFSIZE);
        if (!buf->data) return 1;
        
        (buf -> data)[0] = '\0';
        buf -> used = 1;
        
        return 0;
}

void sb_free(strbld_t* buf) {
        free(buf->data);
}

int sb_append(strbld_t* buf, char item) {
        if (buf->used == buf->size) {
                size_t newSize = buf->size * 2;
                char* temp = realloc(buf->data, newSize);
                
                if (!temp) return 1;
                
                buf->data = temp;
                buf->size = newSize;
        }
        
        (buf->data)[buf->used - 1] = item;
        (buf->data)[buf->used] = '\0';
        buf->used = buf->used + 1;
        
        return 0;
}