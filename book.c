#include "book.h"
#include "student.h"

struct student roster[MAX_S];

int numA = 0;
int numE = 0;
int numS = 0;

int readText()
{
  umask(0);
  int rd = open("gradebook.txt", O_RDONLY | O_CREAT, 0777);
  if(rd < 0)
    return -1;

  printf("Reading from file.\n");
  char buffer[4096];
  if(read(rd, buffer, 4095) < 0)
    return -1;
  printf("Splitting file into lines.\n");

//split txt into lines
  char * txt[MAX_S+2];
  int i = 0;
  txt[i] = strtok(buffer, "\n");
  while(txt[i] != NULL)
  {
    printf("%s\n", txt[i]);
    ++i;
    txt[i] = strtok(NULL, "\n");
  }

  printf("Splitting each line into words/nums.\n");

//i = num lines
//split each word in a line by placed nulls, starting at 3rd line--beginning of report
  char * line[i];
  for(int j = 0; j < i; ++j)
  {
    int k = 0;
    line[k] = strtok(txt[j], " \t\n\r");  //words/vals
    while(line[k] != NULL)
    {
      printf("%s\n", line[k]);
      ++k;
      line[k] = strtok(NULL, " \t\n\r");
    }

    if(j == 0)
    {
      numA = atoi(line[1]);
      numE = atoi(line[3]);
      numS = atoi(line[5]);
      printf("updated numA: %d, numE: %d, numS: %d\n", numA, numE, numS);
    }
    if(j >= 2)
    {
      roster[j-2].name[0] = '\0';
      strncpy(roster[j-2].name, line[0], 19);
      printf("Updated name: %s\n", roster[j-2].name);
      int a;
      for(a = 0; a < numA; ++a)
      {
        roster[j-2].assignments[a] = atof(line[a+1]);
        printf("Updated a%d: %f", (a+1), roster[j-2].assignments[a]);
      }
      for(int e = 0; e < numE; ++e)
      {
        roster[j-2].exams[e] = atof(line[e+1+a]);
        printf("Updated e%d: %f", (e+1), roster[j-2].assignments[e]);
      }
    }
  }
  printf("Finished updating roster.\n");
  return 0;
}

void updateText()
{
  umask(0);
  int wr = open("gradebook.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777);
  char buffer[300];
  snprintf(buffer, 27, "NumA: %d NumE: %d NumS: %d\n", numA, numE, numS);

  write(wr, buffer, strlen(buffer));
  char grade[10];
  bzero(grade, 10);
  bzero(buffer, 300);
  strncpy(buffer, "Students", 8);
  for(int j = 0; j < numA; ++j)
  {
    snprintf(grade, 10, "\tA%d", (j+1));
    strncat(buffer, grade, strlen(grade));
    bzero(grade, 10);
  }
  for(int j = 0; j < numE; ++j)
  {
    snprintf(grade, 10, "\tE%d", (j+1));
    strncat(buffer, grade, strlen(grade));
    bzero(grade, 10);
  }

  strncat(buffer, "\tFinal\n", 7);
  write(wr, buffer, strlen(buffer));

  for(int i = 0; i < numS; ++i)
  {
    bzero(buffer, 300);
    bzero(grade, 10);
    snprintf(buffer, 15, "%-15s", roster[i].name);
    for(int j = 0; j < numA; ++j)
    {
      snprintf(grade, 10, "\t%.2f", roster[i].assignments[j]);
      strncat(buffer, grade, strlen(grade));
      bzero(grade, 10);
    }
    for(int j = 0; j < numE; ++j)
    {
      snprintf(grade, 10, "\t%.2f", roster[i].exams[j]);
      strncat(buffer, grade, strlen(grade));
      bzero(grade, 10);
    }
    double final = calcGrade(i);
    snprintf(grade, 10, "\t%.2f", final);
    strncat(buffer, grade, strlen(grade));
    strncat(buffer, "\n", 1);
    write(wr, buffer, strlen(buffer));
  }
  bzero(buffer, 300);
  strncpy(buffer, "\n", 1);
  write(wr, buffer, 1);


}

void printD(int fd)
{

  char buffer[300], grade[10];
  bzero(grade, 10);
  bzero(buffer, 300);
  strncpy(buffer, "\n\rStudents", 10);
  for(int j = 0; j < numA; ++j)
  {
    snprintf(grade, 10, "\tA%d", (j+1));
    strncat(buffer, grade, strlen(grade));
    bzero(grade, 10);
  }
  for(int j = 0; j < numE; ++j)
  {
    snprintf(grade, 10, "\tE%d", (j+1));
    strncat(buffer, grade, strlen(grade));
    bzero(grade, 10);
  }

  strncat(buffer, "\tFinal\n\r\n\r", 10);
  write(fd, buffer, strlen(buffer));

  for(int i = 0; i < numS; ++i)
  {
    bzero(buffer, 300);
    bzero(grade, 10);
    snprintf(buffer, 15, "%-15s", roster[i].name);
    for(int j = 0; j < numA; ++j)
    {
      snprintf(grade, 10, "\t%.2f", roster[i].assignments[j]);
      strncat(buffer, grade, strlen(grade));
      bzero(grade, 10);
    }
    for(int j = 0; j < numE; ++j)
    {
      snprintf(grade, 10, "\t%.2f", roster[i].exams[j]);
      strncat(buffer, grade, strlen(grade));
      bzero(grade, 10);
    }
    double final = calcGrade(i);
    snprintf(grade, 10, "\t%.2f", final);
    strncat(buffer, grade, strlen(grade));
    strncat(buffer, "\n\r", 2);
    write(fd, buffer, strlen(buffer));
  }
  bzero(buffer, 300);
  strncpy(buffer, "\n\r", 2);
  write(fd, buffer, 2);

}

int addAtB()
{
  if(numA == MAX_A)
  {
    return -1;
  }
  for(int i = 0; i < numS; ++i)
  {
    roster[i].assignments[numA] = 0.0;
  }
  ++numA;

  updateText();
  return 0;
}

int addEtB()
{
  if(numE == MAX_E)
  {
    return -1;
  }

  for(int i = 0; i < numS; ++i)
  {
    roster[i].exams[numE] = 0.0;
  }

  ++numE;

  updateText();
  return 0;
}

int addStR(char * name)
{
  if(numS == MAX_S)
  {
    return -1;
  }
  roster[numS].name[0] = '\0';
  strncpy(roster[numS].name, name, 19);

  for(int i = 0; i < numA; ++i)
    roster[numS].assignments[i] = 0.0;

  for(int i = 0; i < numE; ++i)
    roster[numS].exams[i] = 0.0;

  ++numS;
  updateText();

  return 0;
}

int updateGradeA(int sockfd, int index)
{
  char message[75], gradeS[10];
  message[0] = '\0';
  gradeS[0] = '\0';

  if(index < 0 || index > numA-1)
  {
    strncpy(message, "Assignment does not exist.\n\r", 28);
    write(sockfd, message, strlen(message));
    return -1;
  }
  double grade;
  int n = index +1;
  for(int i = 0; i < numS; ++i)
  {
    bzero(message, 75);
    bzero(gradeS, 10);

    snprintf(message, 56, "Enter %s's grade for assignment %d:  ", roster[i].name, n);

    write(sockfd, message, strlen(message));
    read(sockfd, gradeS, 9);

    if(empty(gradeS))
      read(sockfd, gradeS, 9);

    grade = atof(gradeS);
    if(grade < 0 || grade > 100)
    {
      bzero(message, 75);
      strncpy(message, "Invalid grade.\n\r", 16);
      write(sockfd, message, strlen(message));
    }
    else
      roster[i].assignments[index] = grade;
  }

  updateText();
}

int updateGradeE(int sockfd, int index)
{
  char message[75], gradeS[10];
  message[0] = '\0';
  gradeS[0] = '\0';

  if(index < 0 || index > numE-1)
  {
    strncpy(message, "Exam does not exist.\n\r", 22);
    write(sockfd, message, strlen(message));
    return -1;
  }
  double grade;
  int n = index +1;
  for(int i = 0; i < numS; ++i)
  {
    bzero(message, 75);
    bzero(gradeS, 10);

    snprintf(message, 50, "Enter %s's grade for exam %d: ", roster[i].name, n);
    write(sockfd, message, strlen(message));
    read(sockfd, gradeS, 9);

    //check to make sure not accidentally empty
    if(empty(gradeS))
      read(sockfd, gradeS, 9);

    grade = atof(gradeS);
    if(grade < 0 || grade > 100)
    {
      bzero(message, 75);
      strncpy(message, "Invalid grade.\n\r", 16);
      write(sockfd, message, strlen(message));
    }
    else
      roster[i].exams[index] = grade;
  }

  updateText();
}

int updateGrade(int sockfd, int n)
{
  char message[75], gradeS[10];
  message[0] = '\0';
  gradeS[0] = '\0';
  for(int i = 0; i < numA; ++i)
  {
    double grade;
    int j = i+1, valid = 0;
    bzero(message, 75);
    bzero(gradeS, 10);
    snprintf(message, 32, "Grade for assignment %d: %.2f\n\r", j, roster[n].assignments[i]);

    //if invalid entry, will need to reprompt
    do
    {
      strncat(message, "Enter a new grade: ", 19);
      write(sockfd, message, strlen(message));

      read(sockfd, gradeS, 9);
      //check to make sure not leftover whitespace
      if(empty(gradeS))
        read(sockfd, gradeS, 9);

      //if intentionally empty, skip, don't update grade
      if(!empty(gradeS))
      {
        grade = atof(gradeS);

        if(grade < 0 || grade > 100)
        {
          bzero(message, 75);
          strncpy(message, "Invalid grade.\n\r", 16);
          write(sockfd, message, strlen(message));
          bzero(message, 75);
          bzero(gradeS, 10);
        }
        else
        {
          valid = 1;
          roster[n].assignments[i] = grade;
        }
      }
      else
        valid = 1;
    }while(!valid);
  }

  for(int i = 0; i < numE; ++i)
  {
    double grade;
    int j = i +1, valid = 0;
    bzero(message, 75);
    bzero(gradeS, 10);

    snprintf(message, 25, "Grade for exam %d: %.2f\n\r", j, roster[n].exams[i]);

    //if invalid entry, will need to reprompt
    do
    {
      strncat(message, "Enter a new grade: ", 19);
      write(sockfd, message, strlen(message));

      read(sockfd, gradeS, 9);
      //sometimes carryover whitespace
      if(empty(gradeS))
        read(sockfd, gradeS, 9);

      //if nothing entered, skip
      if(!empty(gradeS))
      {
        grade = atof(gradeS);

        if(grade < 0 || grade > 100)
        {
          bzero(message, 75);
          strncpy(message, "Invalid grade.\n\r", 16);
          write(sockfd, message, strlen(message));
          bzero(message, 75);
          bzero(gradeS, 10);
        }
        else
        {
          valid = 1;
          roster[n].exams[i] = grade;
        }
      }
      else
        valid = 1;
    }while(!valid);

  }

  updateText();
}

int findS(char * name)
{
  char c1 = name[0];
  for(int i = 0; i < numS; ++i)
  {
    char c2 = roster[i].name[0];
    //check capitalization, then rest
    if(tolower(c1) == tolower(c2) && strlen(name) == strlen(roster[i].name))
    {
      for(int j = 1; j < strlen(name); ++j)
      {
        if(name[j] != roster[i].name[j])
          return -1;
      }
      return i;
    }
  }
  return -1;
}

int empty(char buf[])
{
  int flag = 1;
  for(int i = 0; i < strlen(buf); ++i)
    flag = flag && isspace(buf[i]);

  return flag;
}

double calcGrade(int n)
{
  double gradeA = 0.0, gradeE = 0.0;

  if(numA > 0)
  {
    for(int i = 0; i < numA; ++i)
      gradeA += roster[n].assignments[i];
    gradeA = gradeA/numA;
    if(numE == 0)
      return gradeA;
  }
  if(numE > 0)
  {
    for(int i = 0; i < numE; ++i)
      gradeE += roster[n].exams[i];
    gradeE = gradeE/numE;
    if(numA == 0)
      return gradeE;
  }

  return (gradeA + gradeE)/2;
}
