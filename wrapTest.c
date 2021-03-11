#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <fcntl.h>
#include "wrap.h"
int main (int argc, char** argv) {
        int fd = open(argv[1], O_RDONLY);
        size_t lineWidth = atoi(argv[2]);
        wrap(lineWidth, fd, 1);
}