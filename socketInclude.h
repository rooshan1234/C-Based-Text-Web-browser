#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/file.h>

#define TCP_PORTNO 80 /* Pick a port number > 5000 that is not in use */
#define MAX_LINE_SIZE 1000 
#define REQUEST_SIZE 256 
#define HISTORY 20
char *pname;

struct requestreturn
{
	char host_name [REQUEST_SIZE];
	char page_name [REQUEST_SIZE];
};

struct requestreturn history [HISTORY];

void send_message (int socket_fd, int total, char *request);
void read_message(int socket_fd);
