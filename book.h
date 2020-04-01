#ifndef BOOK
#define BOOK
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
//#include "student.h"
#define MAX_S 50
#define MAX_A 20
#define MAX_E 10

int numA;
int numE;
int numS;

int readText();
void updateText();
void printD(int fd);
int addAtB();
int addEtB();
int addStR(char * name);
int updateGradeA(int sockfd, int index);
int updateGradeE(int sockfd, int index);
int updateGrade(int sockfd, int n);
int findS(char * name);
int empty(char buf[]);
double calcGrade(int n);

#endif
