#ifndef SOCK_H
#define SOCK_H


#include <stdio.h>
#include <stdint.h>
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


int64_t sock_create(struct addrinfo *p);
void sock_bind(int64_t sockfd, struct addrinfo *p);
void sock_listen(int64_t sockfd, int64_t backlog);
int64_t sock_accept(int64_t sockfd, struct sockaddr_storage *their_addr, char *s);
int64_t sock_read (int64_t socketd, char *buf, int64_t n);
int64_t sock_write (int64_t socketd, char *buf, int64_t n);
int64_t sock_close (int64_t socketd);


/*
 * The following routines add functionality beyond
 * the basic socket routines.
 *
 */


int64_t sock_readchrd (int64_t socketd, char *buf, int64_t n, char delim);
int64_t sock_readstrd (int64_t socketd, char *buf, int64_t n, char *delim);

void *sock_get_in_addr(struct sockaddr *sa);
#endif
