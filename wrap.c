#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <fcntl.h>
#include "strbld.h"


#define INTERNAL_BUFSIZE 8

int wrap(size_t lineWidth, int inputStream, int outputStream) {
        char buffer[INTERNAL_BUFSIZE];
        
        strbld_t word;
        int ret;
        
        ret = sb_init(&word);
        if (ret) return 1;
        
        bool newlineRead = false;
        bool canParagraph = false;
        bool oversizedLine = false;
        bool startOfLine = true;
        size_t charsWritten = 0;
        size_t bytesRead = 0;
        
        char newlineBuf[1] = {'\n'};
        char spaceBuf[1] = {' '};
        
        while (bytesRead = read(inputStream, buffer, INTERNAL_BUFSIZE)) {
                for (int i = 0; i < bytesRead; i++) {
                        char temp = buffer[i];
                        if (!isspace(temp)) {
                                ret = sb_append(&word, temp);
                                if (ret) return 1;
                                newlineRead = false;
                                canParagraph = true;
                        } else if (word.used != 1) {
                                size_t wordLength;
                                wordLength = (startOfLine) ? (word.used - 1) : (word.used);
                                size_t spaceRemaining = lineWidth - charsWritten;
                                
                                if (wordLength <= spaceRemaining) {
                                        if (!startOfLine) {
                                                write(outputStream, spaceBuf, 1);
                                        }
                                        write(outputStream, word.data, wordLength);
                                        charsWritten += wordLength;
                                        sb_free(&word);
                                        ret = sb_init(&word);
                                        if (ret) return 1;
                                        
                                        startOfLine = false;
                                        
                                } else if (wordLength > lineWidth) {
                                        write(outputStream, word.data, wordLength);
                                        oversizedLine = true;
                                        startOfLine = true;
                                        charsWritten = 0;
                                        
                                } else {
                                        write(outputStream, newlineBuf, 1);
                                        charsWritten = 0;
                                        
                                        wordLength = word.used - 1;
                                        write(outputStream, word.data, wordLength);
                                        charsWritten += wordLength;
                                        
                                        sb_free(&word);
                                        ret = sb_init(&word);
                                        if (ret) return 1;
                                        
                                        startOfLine = false;
                                }
                        }
                                
                        if (temp == '\n' && newlineRead && canParagraph) {
                                write(outputStream, newlineBuf, 1);
                                write(outputStream, newlineBuf, 1);
                                newlineRead = false;
                                canParagraph = false;
                                startOfLine = true;
                                charsWritten = 0;
                                        
                        } else if (temp == '\n') {
                                newlineRead = true;
                        }
                }
        }
        
        write(outputStream, newlineBuf, 1);
        
        if (oversizedLine) {
                return 1;
        } else {
                return 0;
        }
}

int main (int argc, char** argv) {
        int fd = open(argv[1], O_RDONLY);
        wrap(atoi(argv[2]), fd, 1);
}