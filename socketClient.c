#include "socketInclude.h"

int main (int argc, char **argv)
{
		int i,x,socket_fd, trace_flag = 0;
		long int pid, net_pid;
		time_t current_time;
		unsigned long int host_id;
		struct sockaddr_in server_addr;
		struct hostent *hp;
		struct in_addr client_addr;

		char host_name [256];
		char page_name [256];
		char server_host_name[256];

		char request [1000];
		char user_input [20]; 
		int print_request_urls = 1;
		current_time = time (NULL);

		int index = 0;
		int length = 0;
		int temp_index = 0;

		//main loop
		do{
				printf("Enter the action you would like to preform (PRINT, NOPRINT, HISTORY, CLEAR, HOME, BACK, FOWARD, GO, STOP): ");
				scanf("%19s", user_input);
				if (strcmp ("PRINT", user_input) == 0 ){
						printf("All subsequent requests will now output the document associated with the specfied request-URLs.\n");
						print_request_urls = 1;
				}else if (strcmp("NOPRINT", user_input) == 0){
						printf("All subsequent requests will NOT output the document associated with the specfied request-URLs.\n");
						print_request_urls = 0;
				}else if (strcmp("HISTORY", user_input) == 0){
						//print history list
						printf("Page History: \n");
						for (i = 0; i < length; i++){
								if (index == i){
										printf("current page -->   ");
								}	
								printf("%s %s\n", history[i].host_name, history[i].page_name);			
						}	
				}else if (strcmp("CLEAR", user_input) == 0){
						for (i = 0; i < length; i++){
							for (x = 0; x < REQUEST_SIZE; x++){
								history[i].host_name[x] = '\0';
								history[i].page_name[x] = '\0'; 
							}
						}
						length = index = 0;
						printf ("History cleared.\n");
				}else if (strcmp("HOME", user_input) == 0){
						//go to the first page in history, and add it again to the list
						index = 0;
						printf("INDEX IS AT: %i: \n", index);
						printf("HISTORY IS AT: %s, %s \n", history[length - 1].host_name, history[length - 1].page_name);
						if (length > 0){

						if ((socket_fd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
						{
							printf ("socket ERROR in main");
							exit (1);
						}

						/*
						   Establish the remote address and port number of the server.
						   */

						memset (&server_addr, 0, sizeof (server_addr));
						server_addr.sin_family = AF_INET;
						//change argv[1] no longer valid
						hp = gethostbyname (history[index].host_name);
						if (hp == NULL)
						{
								printf ("Gethostbyname SERVER ERROR in main: %s does not exist.\n", history[index].host_name);
								continue;
						}
						host_id = *((unsigned long int *) (hp -> h_addr_list));
						memcpy (&server_addr.sin_addr, hp -> h_addr, hp -> h_length);
						server_addr.sin_port = htons (TCP_PORTNO);

						/*
						   Establish the connection between the client and server processes.  If an
						   error occurs, print an error message and quit the program.
						   */

						if (connect (socket_fd, (struct sockaddr *) &server_addr, sizeof (server_addr)) < 0)
						{
								printf ("connect ERROR in main");
								exit (1);
						}

						/*
						   Establish the local address and process id.
						   */

						gethostname (host_name, sizeof (host_name));
						hp = gethostbyname (host_name);
						if (hp == NULL)
						{
								printf ("gethostbyname LOCAL ERROR in main: %s does not exist", host_name);
								exit (1);
						}
						host_id = *((unsigned long int *) (hp -> h_addr_list));
						memcpy (&host_id, hp -> h_addr, sizeof (host_id));
						pid = getpid ();

						//creating the request
						
						if (print_request_urls){
							sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",history[index].page_name,history[index].host_name);  
						}else{
							sprintf(request, "HEAD %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",history[index].page_name,history[index].host_name);  
						}
						
						printf("REQUEST SEND: \n%s", request);
						//assuming no errors at this point, so add to history, history will only store valid links
						for (i = 0; i < REQUEST_SIZE; i++){
								history[length].host_name[i] = server_host_name[i];
								history[length].page_name[i] = page_name[i];
						}
						length++;

						int request_length = strlen(request);

						send_message(socket_fd, request_length, request);

						read_message(socket_fd);

						close (socket_fd);
						/******************************* CONNECTION END ***********************************************/

					}
				}else if (strcmp("BACK", user_input) == 0){
						//go back in the page history
						temp_index = index;
						temp_index--;
						if (temp_index <= 0){
								printf("Cannot go further back in history\n");
								index = 0;	
						}else{
								index = temp_index;
						}
						
						printf("INDEX IS AT: %i: \n", index);
						printf("HISTORY IS AT: %s, %s \n", history[index].host_name, history[index].page_name);


						/******************** CONNECTION ************************/
						if ((socket_fd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
						{
								printf ("socket ERROR in main");
								exit (1);
						}

						/*
						   Establish the remote address and port number of the server.
						   */

						memset (&server_addr, 0, sizeof (server_addr));
						server_addr.sin_family = AF_INET;
						//change argv[1] no longer valid
						hp = gethostbyname (history[index].host_name);
						if (hp == NULL)
						{
								printf ("Gethostbyname SERVER ERROR in main: %s does not exist.\n", history[index].host_name);
								continue;
						}
						host_id = *((unsigned long int *) (hp -> h_addr_list));
						memcpy (&server_addr.sin_addr, hp -> h_addr, hp -> h_length);
						server_addr.sin_port = htons (TCP_PORTNO);

						/*
						   Establish the connection between the client and server processes.  If an
						   error occurs, print an error message and quit the program.
						   */

						if (connect (socket_fd, (struct sockaddr *) &server_addr, sizeof (server_addr)) < 0)
						{
								printf ("connect ERROR in main");
								exit (1);
						}

						/*
						   Establish the local address and process id.
						   */

						gethostname (host_name, sizeof (host_name));
						hp = gethostbyname (host_name);
						if (hp == NULL)
						{
								printf ("gethostbyname LOCAL ERROR in main: %s does not exist", host_name);
								exit (1);
						}
						host_id = *((unsigned long int *) (hp -> h_addr_list));
						memcpy (&host_id, hp -> h_addr, sizeof (host_id));
						pid = getpid ();

						//creating the request
						
						if (print_request_urls){
							sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",history[index].page_name,history[index].host_name);  
						}else{
							sprintf(request, "HEAD %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",history[index].page_name,history[index].host_name);  
						}
						
						printf("REQUEST SEND: \n%s", request);
						//assuming no errors at this point, so add to history, history will only store valid links

						int request_length = strlen(request);

						send_message(socket_fd, request_length, request);

						read_message(socket_fd);

						close (socket_fd);
						/******************************* CONNECTION END ***********************************************/
					
					}else if (strcmp("FORWARD", user_input) == 0){
						//go foward in history

						temp_index = index;
						//index = (index + 1) % length; loop aroung method
						temp_index++;
						if (temp_index >= length){
								printf("Cannot go further forward in history\n");
								if (length > 0){
									index = length - 1;
								}
						}else {
								index = temp_index;	
						}
						printf("INDEX IS AT: %i: \n", index);
						printf("HISTORY IS AT: %s, %s \n", history[index].host_name, history[index].page_name);
					
						/******************** CONNECTION ************************/
						if ((socket_fd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
						{
								printf ("socket ERROR in main");
								exit (1);
						}

						/*
						   Establish the remote address and port number of the server.
						   */

						memset (&server_addr, 0, sizeof (server_addr));
						server_addr.sin_family = AF_INET;
						//change argv[1] no longer valid
						hp = gethostbyname (history[index].host_name);
						if (hp == NULL)
						{
								printf ("Gethostbyname SERVER ERROR in main: %s does not exist.\n", history[index].host_name);
								continue;
						}
						host_id = *((unsigned long int *) (hp -> h_addr_list));
						memcpy (&server_addr.sin_addr, hp -> h_addr, hp -> h_length);
						server_addr.sin_port = htons (TCP_PORTNO);

						/*
						   Establish the connection between the client and server processes.  If an
						   error occurs, print an error message and quit the program.
						   */

						if (connect (socket_fd, (struct sockaddr *) &server_addr, sizeof (server_addr)) < 0)
						{
								printf ("connect ERROR in main");
								exit (1);
						}

						/*
						   Establish the local address and process id.
						   */

						gethostname (host_name, sizeof (host_name));
						hp = gethostbyname (host_name);
						if (hp == NULL)
						{
								printf ("gethostbyname LOCAL ERROR in main: %s does not exist", host_name);
								exit (1);
						}
						host_id = *((unsigned long int *) (hp -> h_addr_list));
						memcpy (&host_id, hp -> h_addr, sizeof (host_id));
						pid = getpid ();


						//creating the request
						if (print_request_urls){
							sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",history[index].page_name,history[index].host_name);  
						}else{
							sprintf(request, "HEAD %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",history[index].page_name,history[index].host_name);  
						}
						
						printf("REQUEST SEND: \n%s", request);

						//assuming no errors at this point, so add to history, history will only store valid links

						int request_length = strlen(request);

						send_message(socket_fd, request_length, request);

						read_message(socket_fd);

						close (socket_fd);
						/******************************* CONNECTION END ***********************************************/
				}else if (strcmp("GO", user_input) == 0){
						//lauch the connection and send the request
						printf("Please enter the the server host name: ");
						scanf("%255s", server_host_name);

						/*
						   Create a socket descriptor, specify the type of communication protocol
						   (ie. address family and socket type).  If an error occurs, print an error
						   message and quit the program.
						   */

						if ((socket_fd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
						{
								printf ("socket ERROR in main");
								exit (1);
						}

						/*
						   Establish the remote address and port number of the server.
						   */

						memset (&server_addr, 0, sizeof (server_addr));
						server_addr.sin_family = AF_INET;
						//change argv[1] no longer valid
						hp = gethostbyname (server_host_name);
						if (hp == NULL)
						{
								printf ("Gethostbyname SERVER ERROR in main: %s does not exist.\n", server_host_name);
								continue;
						}
						host_id = *((unsigned long int *) (hp -> h_addr_list));
						memcpy (&server_addr.sin_addr, hp -> h_addr, hp -> h_length);
						server_addr.sin_port = htons (TCP_PORTNO);

						/*
						   Establish the connection between the client and server processes.  If an
						   error occurs, print an error message and quit the program.
						   */

						if (connect (socket_fd, (struct sockaddr *) &server_addr, sizeof (server_addr)) < 0)
						{
								printf ("connect ERROR in main");
								exit (1);
						}

						/*
						   Establish the local address and process id.
						   */

						gethostname (host_name, sizeof (host_name));
						hp = gethostbyname (host_name);
						if (hp == NULL)
						{
								printf ("gethostbyname LOCAL ERROR in main: %s does not exist", host_name);
								exit (1);
						}
						host_id = *((unsigned long int *) (hp -> h_addr_list));
						memcpy (&host_id, hp -> h_addr, sizeof (host_id));
						pid = getpid ();

						printf("Please enter the the page name: ");
						scanf("%255s", page_name);


						//creating the request
						if (print_request_urls){
							sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", page_name, server_host_name);  
						}else{
							sprintf(request, "HEAD %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", page_name, server_host_name);  
						}
						
						printf("REQUEST SEND: \n%s", request);

						//assuming no errors at this point, so add to history, history will only store valid links

						for (i = 0; i < REQUEST_SIZE; i++){
								history[length].host_name[i] = server_host_name[i];
								history[length].page_name[i] = page_name[i];
						}
						index = length;	
						length++;
						int request_length = strlen(request);

						send_message(socket_fd, request_length, request);

						read_message(socket_fd);

						close (socket_fd);
				}else if (strcmp ("STOP", user_input) != 0){
						printf("Invalid command, please enter a valid command.\n");
				}
		}while (strcmp ("STOP", user_input) != 0); //run while loop until user types STOP
}

/*
   This function is used to send a message to the server.  Messages are read
   from the standard input device and sent to the server until there are no
   more messages to send.
   */
void send_message (int socket_fd, int total, char *request){

		//send message to socket_fd
		int bytes_sent = 0;
		int total_sent = 0;
		do{
				bytes_sent = write (socket_fd, request + total_sent, total - total_sent);
				if (bytes_sent < 0){
						error ("ERROR writing");
				}
				if (bytes_sent == 0){
						break;
				}
				total_sent+=bytes_sent;
		}while (total_sent < total);
}

void read_message(int socket_fd){
		//read message from server response 
		int characters_read = 0;
		int buffer[10];
		while ((characters_read = read (socket_fd, buffer, 10)) != 0){
				characters_read = write(1, buffer, characters_read);
		}
}
