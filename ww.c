#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
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

bool isNumber(char *string) {
	size_t length = strlen(string);
	for (int i = 0; i < length; i++) {
		if (!isdigit(string[i])) {
			return false;
		}
	}
	return true;
}

int main(int argc, char **argv) {
	int sumWrapErrors = 0;
	if(argc < 2 || argc > 3) {
		write(2, "Invalid arguments\n", 18);
		exit(EXIT_FAILURE);
	}
	if(!isNumber(argv[1])) {
		write(2, "Invalid arguments\n", 18);
		exit(EXIT_FAILURE);
	}
	size_t lineWidth = atoi(argv[1]);
	if(lineWidth < 1) {
		write(2, "Invalid arguments\n", 18);
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
			if(fd == -1) { // error opening file or directory
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
			if(dir == NULL) { // could not open directory (may be already caught by "fd == -1" above)
				perror(argv[2]);
				exit(EXIT_FAILURE);
			}
			struct dirent *dp;
			if(chdir(argv[2]) == -1) {
				perror(argv[2]);
				exit(EXIT_FAILURE);
			}
			while((dp = readdir(dir)) != NULL) {
				if(isdir(dp->d_name) == -1) {
					perror(dp->d_name);
					exit(EXIT_FAILURE);
				}
				else if(isdir(dp->d_name) == 1) { // skip subdirectories
					continue;
				}
				if(strncmp(dp->d_name, ".", 1) == 0 || strncmp(dp->d_name, "wrap.", 5) == 0) { // skip if file name starts with "." or "wrap."
					continue;
				}
				else {
					char newfilename[255] = "wrap.";
					strcat(newfilename, dp->d_name);
					int fd = open(dp->d_name, O_RDONLY);
					int newfd = open(newfilename, O_WRONLY|O_TRUNC|O_CREAT, 0666);
					sumWrapErrors += wrap(lineWidth, fd, newfd);
					close(fd);
					close(newfd);
				}
			}
			closedir(dir);
		}
		else { // neither file nor directory; should never happen
			write(2, "Invalid arguments\n", 18);
			exit(EXIT_FAILURE);
		}
	}
	return(sumWrapErrors);
}
