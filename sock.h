#ifndef SOCK_H
#define SOCK_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>


int sock_create(struct addrinfo *p);
void sock_bind(int sockfd, struct addrinfo *p);
void sock_listen(int sockfd, int backlog);
int sock_accept(int sockfd, struct sockaddr_storage *their_addr, char *s);
int sock_read (int socketd, char *buf, int n);
int sock_write (int socketd, char *buf, int n);
int sock_close (int socketd);


/*
 * The following routines add functionality beyond
 * the basic socket routines.
 *
 */


int sock_readchrd (int socketd, char *buf, int n, char delim);
int sock_readstrd (int socketd, char *buf, int n, char *delim);

void *sock_get_in_addr(struct sockaddr *sa);
#endif
