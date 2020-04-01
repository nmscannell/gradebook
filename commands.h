#ifndef COMMANDS
#define COMMANDS
#include "book.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void addS(int sockfd, char * name);
void addA(int sockfd);
void addE(int sockfd);
void grade(int sockfd, char * name);
void gradeA(int sockfd, char * num);
void gradeE(int sockfd, char * num);
void display(int fd);

#endif
