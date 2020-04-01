#include "commands.h"

void addS(int sockfd, char * name)
{

  char * message;
  for(int i = 0; i < strlen(name); ++i)
    if(!isalpha(name[i]))
    {
      message = "Name cannot contain non-alpha characters.\n\r";
      write(sockfd, message, strlen(message));
      return;
    }

  int n = findS(name);
  if(n >= 0)
  {
    message = "Student already exists.\n\r";
    write(sockfd, message, strlen(message));
    return;
  }

  if(addStR(name) < 0)
  {
    message = "Cannot add more students.\n\r";
    write(sockfd, message, strlen(message));
  }
  else
  {
    message = "Student successfully added.\n\r";
    write(sockfd, message, strlen(message));
  }

}

void addA(int sockfd)
{
  char * message;
  if(addAtB() < 0)
  {
    message = "Cannot add more assignments.\n\r";
    write(sockfd, message, strlen(message));
  }
  else
  {
    message = "Assignment added.\n\r";
    write(sockfd, message, strlen(message));
  }
}

void addE(int sockfd)
{
  char * message;
  if(addEtB() < 0)
  {
    message = "Cannot add more exams.\n\r";
    write(sockfd, message, strlen(message));
  }
  else
  {
    message = "Exam added.\n\r";
    write(sockfd, message, strlen(message));
  }
}

void grade(int sockfd, char * name)
{
  char * message;
  int n = findS(name);
  if(n < 0)
  {
    message = "Student does not exist.\n\r";
    write(sockfd, message, strlen(message));
    return;
  }

  updateGrade(sockfd, n);
}

void gradeA(int sockfd, char * num)
{
  char * message;

  for(int i = 0; i < strlen(num); ++i)
    if(!isdigit(num[i]))
    {
      message = "Invalid command. Need a numeric input.\n\r";
      write(sockfd, message, strlen(message));
      return;
    }

  int index = atoi(num) - 1;

  updateGradeA(sockfd, index);
}

void gradeE(int sockfd, char * num)
{
  char * message;

  for(int i = 0; i < strlen(num); ++i)
    if(!isdigit(num[i]))
    {
      message = "Invalid command. Need a numeric input.\n\r";
      write(sockfd, message, strlen(message));
      return;
    }

  int index = atoi(num) - 1;
  updateGradeE(sockfd, index);
}

void display(int fd)
{
  printD(fd);
}

