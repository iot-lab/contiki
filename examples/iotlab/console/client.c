#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "myencode.h"

#include "cmds.h"

// known commands
int uploadfile (char* params[]);
int noArgumentCommand(char* params[]);
int oneArgumentCommand(char* params[]);
int exitCommand (char* params[]);
int helpCommand (char* params[]);
Command commands[] = {
	{
		.name = "upload",
		.exec = &uploadfile
	},
	{
		.name = "ls",
		.exec = &noArgumentCommand
	},
	{
		.name = "format",
		.exec = &noArgumentCommand
	},
	{
		.name = "cat",
		.exec = &oneArgumentCommand
	},
	{
		.name = "rm",
		.exec = &oneArgumentCommand
	},
	{
		.name = "loadelf",
		.exec = &oneArgumentCommand
	},
	{
		.name = "exit",
		.exec = &exitCommand
	},
	{
		.name = "help",
		.exec = &helpCommand
	}
};

// global variables
static int sockfd = 0;

// global methods
void send_data(char* msg);

// implementing commands
int uploadfile (char* params[]) 
{
	unsigned char buf[4096];
	char out[4096 + 1000];	
	int n;
	sprintf(buf, "upload %s\n", params[1]);
	send_data(buf);
	int fd = open(params[1], O_RDONLY);
	while ((n = read(fd, buf, 4096)) > 0) {
		int r = encode(buf, n, out);
		out[r] = 0;
		send_data(out);
		send_data("\n");
	}
	close(fd);
	send_data("endupload\n");
	return 1;
}
int noArgumentCommand(char* params[])
{
	send_data(params[0]); send_data("\n");
	return 1;
}
int oneArgumentCommand(char* params[])
{
	send_data(params[0]); send_data(" "); send_data(params[1]); send_data("\n");
	return 1;
}
int helpCommand (char* params[])
{
	int nb = sizeof(commands) / sizeof(Command);
	for (int i = 0 ; i < nb ; i++) printf("Command: %s\n", commands[i].name);
	return 1;
}
int exitCommand (char* params[])
{
	close(sockfd);
	exit(0);
}

// working with the sockets
void error(const char *msg)
{
    perror(msg);
	printf("\n");
    exit(1);
}

void send_data(char* msg)
{
	struct timespec req;
	struct timespec done;
	char* s = msg;
	int ended = 0;
	while (!ended) {
		char* sTmp = strstr(s, "\n");
		if (sTmp) {
			sTmp ++;
			write(sockfd,s,(sTmp - s));
			req.tv_sec = 0;
			req.tv_nsec = 300000000; // 500 milliseconds
			nanosleep(&req, &done);
			//sleep(1);
			s = sTmp;
		}
		else {
			if (strlen(s)) { 
				write(sockfd, s, strlen(s));
				req.tv_sec = 0;
				req.tv_nsec = 300000000; // 500 milliseconds
				nanosleep(&req, &done);
				//sleep(1);
			}
			ended = 1;
		}
	}
}

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

void receive_data()
{
	char buff[101];
	int n = read(sockfd,buff,100);
	while (n > 0) {
		for (int i = 0 ; i < n ; i++)
			printf(KGRN "%c", buff[i]);
		n = read(sockfd,buff,100);
	}
}

void *reading_from_remote(void *x_void_ptr)
{

	receive_data();

	/* the function must return something - NULL will do */
	return NULL;
}


// options
#define LOCAL_PORT "-p"
static int localPort = 20000;


static int isSeparator(char c) {
	return c == '\t' || c == '\n' || c == ' ';
}

int main(int argc, char* argv[])
{
	// reading options
	for (int i = 1 ; i < argc ; ) {
		if (!strcmp(argv[i], LOCAL_PORT) && i + 1 < argc) {
			localPort = atoi(argv[++i]);
			i++;
		}
	}

	// connecting to remote devicclose(sockfd);e or server
    struct sockaddr_in serv_addr;
    struct hostent *server;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero((char *) &serv_addr, sizeof(serv_addr));
	if (sockfd < 0)  error("ERROR opening socket");
	serv_addr.sin_family = AF_INET;
	server = gethostbyname("localhost");
	bcopy((char *)server->h_addr, 
	     (char *)&serv_addr.sin_addr.s_addr,
	     server->h_length);
	serv_addr.sin_port = htons(localPort);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
	    error("ERROR connecting");

	printf(KNRM "> ");
	fflush(stdout);

	pthread_t receiver;
	/* create a second thread which executes inc_x(&x) */
	if(pthread_create(&receiver, NULL, reading_from_remote, NULL)) {

		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	char command[100];
	const int nbCommands = sizeof(commands) / sizeof(Command);
	char* params[20];
	for (int i = 0 ; i < 20 ; i ++) params[i] = (char*)malloc(100*sizeof(char));

	while (1) {
			fgets(command, 100, stdin);			
			char* tmp = command;
			int idx = 0;
			while ((*tmp)) {
				int tt = 0;
				while ((*tmp) && !isSeparator(*tmp)) {
					params[idx][tt++] = (*tmp);
					tmp++;				
				}
				params[idx][tt] = 0;
				if (isSeparator(*tmp)) tmp++;
				idx ++;			
			
			}
			for (int i = 0 ; i < nbCommands ; i++) {
				if (strcmp(commands[i].name, params[0]) == 0) {
					commands[i].exec(params);				
				}			
			}
			printf(KNRM "> ");
			fflush(stdout);
	}
	close(sockfd);
	
	return 0;
}
