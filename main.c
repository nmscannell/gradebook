#include "book.h"
#include "commands.h"
#include <sys/wait.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define BUF_SIZE 256

int main(int argc, char **argv)
{
  int port = atoi(argv[1]);

  if(argc != 2 || (port < 11000 && port > 19999))
  {
    printf("Invalid command.\n");
    return -1;
  }

  int listenSock, connectSock, lengthOfClientAddress;
  char buffer[BUF_SIZE];
  struct sockaddr_in serverAddress, clientAddress;

  listenSock = socket(AF_INET, SOCK_STREAM, 0);

  if(listenSock < 0)
  {
    printf("Error opening listening socket.\n");
    return -1;
  }

  bzero((char *) &serverAddress, sizeof(serverAddress));

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(port);

  if(bind(listenSock, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0)
  {
    printf("Error binding to socket.\n");
    return -1;
  }

  listen(listenSock, 3); //how many in queue?
  lengthOfClientAddress = sizeof(clientAddress);

  while(1)
  {
    connectSock = accept(listenSock, (struct sockaddr *)&clientAddress, &lengthOfClientAddress);
    if(connectSock < 0)
    {
      printf("Error accepting.\n");
      return -1;
    }
    if(fork())
    {
      close(connectSock);
    }
    else
    {
      close(listenSock);
      if(readText() < 0)
        write(connectSock, "Unable to upload from file.\n",28); 
      char prompt[] = "> ";
      write(connectSock, prompt, 2);

      while(1)
      {
        bzero(buffer, BUF_SIZE);
        int n = read(connectSock, buffer, BUF_SIZE-1);
        if(n < 0)
        {
          printf("Error reading from socket.\n");
          exit(1);
        }

        if(!empty(buffer))
        {
          char * com[3];
          int i = 0;
          com[i] = strtok(buffer, " ");
          while(com[i] != NULL)
          {
            ++i;
            com[i] = strtok(NULL, " ");
          }

          for(int j = 0; j < strlen(com[i-1]); ++j)
            if(isspace(com[i-1][j]))
              com[i-1][j] = '\0';

          char * command = com[0];
          char * invalid = "Invalid command.\n\r";
          if(i == 1) //only 1 arg
          {
            if(strlen(command) == 1 && strncmp(command, "q", strlen(command)) == 0)
            {
              shutdown(connectSock, SHUT_RDWR);
              close(connectSock);
              exit(1);
            }
            else if(strlen(command) == 4 && strncmp(command, "addA", strlen(command)) == 0)
              addA(connectSock);

            else if(strlen(command) == 4 && strncmp(command, "addE", strlen(command)) == 0)
              addE(connectSock);

            else if(strlen(command) == 7 && strncmp(command, "display", strlen(command)) == 0)
              display(connectSock);
            else
              write(connectSock, invalid, strlen(invalid));
          }
          else if(i == 2) //2 args
          {
            if(strlen(command) == 4 && strncmp(command, "addS", strlen(command)) == 0) //only first?
              addS(connectSock, com[1]);

            else if(strlen(command) == 5 && strncmp(command, "grade", strlen(command)) == 0)
              grade(connectSock, com[1]);

            else if(strlen(command) == 6 && strncmp(command, "gradeA", strlen(command)) == 0)
              gradeA(connectSock, com[1]);

            else if(strlen(command) == 6 && strncmp(command, "gradeE", strlen(command)) == 0)
              gradeE(connectSock, com[1]);

            else
              write(connectSock, invalid, strlen(invalid));
          }
          else
            write(connectSock, invalid, strlen(invalid));

          char prompt[] = "> ";
          write(connectSock, prompt, 2);
        }
      }

    }
  }
  return 0;
}

