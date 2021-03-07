#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "strbld.h"

int main (int argc, char** argv) {
        strbld_t buf;
        sb_init(&buf);
        
        while (1) {
               printf("\n");
               char userInput;
               int ret;
               
               ret = scanf("%c", &userInput);
               if (ret == 0) {
                       sb_free(&buf);
                       break;
               }
               ret = sb_append(&buf, userInput);
        }
}