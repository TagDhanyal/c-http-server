#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h> 

#define DEFAULT_PORT 8080
#define MAX_CLIENTS 10
#define PORT 8081 

exert struct sockaddr_in serverAddr;

typedef struct
{
  char *header[256];
  int error;
}response;


#endif /* SERVER_H */