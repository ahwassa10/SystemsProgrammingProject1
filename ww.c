#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "wrap.h"

int isdir(char *name) {
	struct stat data;
	int err = stat(name, &data);
	if(err) {
		perror(name);
		return -1;
	}
	if(S_ISDIR(data.st_mode)) {
		return 1;
	}
	return 0;
}

int isfile(char *name) {
	struct stat data;
	int err = stat(name, &data);
	if(err) {
		perror(name);
		return -1;
	}
	if(S_ISREG(data.st_mode)) {
		return 1;
	}
	return 0;
}

int main(int argc, char **argv) {
	if(argc < 2 || argc > 3) {
		perror("invalid arguments");
		exit(EXIT_FAILURE);
	}
	size_t lineWidth = atoi(argv[1]);
	if(lineWidth < 1) {
		exit(EXIT_FAILURE);
	}
	if(argc == 2) { // ww lineWidth
		if(wrap(lineWidth, 0, 1) != 0) {
			exit(EXIT_FAILURE);
		}
	}
	else if(argc == 3) {
		if(isfile(argv[2]) == -1) {
			exit(EXIT_FAILURE);
		}
		else if(isfile(argv[2]) == 1) { // ww lineWidth file
			int fd = open(argv[2], O_RDONLY);
			if(fd == -1) { // error opening file
				perror(argv[2]);
				exit(EXIT_FAILURE);
			}
			if(wrap(lineWidth, fd, 1) != 0) {
				exit(EXIT_FAILURE);
			}
			close(fd);
			return 0;
		}
		else if(isdir(argv[2]) == 1) {// ww lineWidth directory
			DIR *dir = opendir(argv[2]);
			//TODO
			closedir(dir);
		}
	}
}
