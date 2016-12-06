#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_MESSAGE_SIZE 1000
#define MAX_NICKNAME_SIZE 100

int sockfd;
int n, len;
char sendline[MAX_MESSAGE_SIZE], recvline[MAX_MESSAGE_SIZE], nickname[MAX_NICKNAME_SIZE];
struct sockaddr_in servaddr, cliaddr;

void send_message(char * message, char * sender, struct sockaddr_in recipient) {
	if (sendto(sockfd, sender, strlen(sender) + 1, 0, (struct sockaddr*)&recipient, sizeof(recipient)) < 0) {
		perror(NULL);
		close(sockfd);
		exit(1);
	}
	
	if (sendto(sockfd, message, strlen(message) + 1, 0, (struct sockaddr*)&recipient, sizeof(recipient)) < 0) {
		perror(NULL);
		close(sockfd);
		exit(1);
	}
}

int main(int argc, char** argv)
{
  
	if (argc != 3)
	{
		printf("Usage: ./a.out <IP address> <Nickname>\n");
		exit(1);
	}
	if ( !strcmp(argv[2], "SERVER") ) {
		printf("You can't use this nickname! Please, select another\n");
		exit(0);
	}
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror(NULL);
		exit(1);
	}
  
	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(0);
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
	if (bind(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(1);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51000);
	if (inet_aton(argv[1], &servaddr.sin_addr) == 0)
	{
		printf("Invalid IP address\n");
		close(sockfd);
		exit(1);
	}
	pid_t pid = fork();
	if (pid != 0) {
		while(1) 
		{
			//printf("You -> ");
			fgets(sendline, MAX_MESSAGE_SIZE, stdin);
			send_message(sendline, argv[2], servaddr);
		}
	}
	else {
		while(1) {
			if ((n = recvfrom(sockfd, nickname, MAX_MESSAGE_SIZE - 1, 0, (struct sockaddr*) NULL, NULL)) < 0)
			{
				perror(NULL);
				close(sockfd);
				exit(1);
			}
			
			if ((n = recvfrom(sockfd, recvline, MAX_MESSAGE_SIZE - 1, 0, (struct sockaddr*) NULL, NULL)) < 0)
			{
				perror(NULL);
				close(sockfd);
				exit(1);
			}
			if ( strcmp(argv[2], nickname) ) {
				printf("%s -> ", nickname);	
				printf("%s", recvline);
			}
		}
	}
	close(sockfd);
	return 0;
}
