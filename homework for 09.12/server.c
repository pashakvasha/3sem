#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_MESSAGE_SIZE 1000
#define MAX_NICKNAME_SIZE 100
#define MAX_USERS_AMOUNT 10

typedef struct User {
	struct in_addr ip;
	unsigned short port;
	char* nickname;
} User;

User users[MAX_USERS_AMOUNT];
char line[MAX_MESSAGE_SIZE];
char nick[MAX_NICKNAME_SIZE];

int last_user;
int sockfd;
int clilen, n;
struct sockaddr_in servaddr, cliaddr, useraddr;

void create_sockaddr(struct sockaddr_in * sockaddr, in_addr_t ip, unsigned short port) {
	bzero(sockaddr, sizeof(sockaddr));
	sockaddr->sin_family = AF_INET;
	sockaddr->sin_port = port;
	sockaddr->sin_addr.s_addr = ip;
}

int search_user(char * nickname) {
	int i;
	for (i = 0; i < last_user; i++)
	{
		if ( !( strcmp(users[i].nickname, nickname) ) ) {
			return i;
		}
	}
	return -1;
}

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

void send_to_all(char * message, char * sender) {
	int i;
	for (i = 0; i < last_user; i++) {
		create_sockaddr(&useraddr, users[i].ip.s_addr, users[i].port);
		send_message(message, sender, useraddr);
	}
}

User sign_up(unsigned short port, struct in_addr ip, char * nickname) {
	User user;
	int k = search_user(nickname);
	if (k != -1) {
		user = users[k];
	}
	else {
		users[last_user].nickname = calloc(MAX_NICKNAME_SIZE, sizeof(char));
		users[last_user].nickname = strcpy(users[last_user].nickname, nickname);
		users[last_user].ip = ip;
		users[last_user].port = port;
		user = users[last_user];
		create_sockaddr(&useraddr, users[last_user].ip.s_addr, users[last_user].port);
		send_message("Welcome to messenger! Use user_nickname@ to send private message.\n", "SERVER", useraddr);
		send_to_all( strcat(nickname, " HAS SIGNED UP\n"), "SERVER");
		printf("%s HAS SIGNED UP\n", users[last_user].nickname);
		last_user++;
	}
	return user;
}

int is_private(char * message) {
	int i, message_size = sizeof(message);
	for (i = 0; i < message_size; i++) {
		if (message[i] == '@') {
			return i;
		}
	}
	return 0;
}

int main()
{
	int i, k;

	create_sockaddr(&servaddr, htonl(INADDR_ANY), htons(51000));
  
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror(NULL);
		exit(1);
	} else
	{
		printf("Sock fd:%d\n", sockfd);
	}
  
	if (bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(1);
	}
	
	while (1)
	{
		clilen = sizeof(cliaddr);
		if ((n = recvfrom(sockfd, line, MAX_MESSAGE_SIZE - 1, 0, (struct sockaddr*)&cliaddr, &clilen)) < 0)
		{
			perror(NULL);
			close(sockfd);
			exit(1);
		}
		
		User user = sign_up(cliaddr.sin_port, cliaddr.sin_addr, line);
		
		if ((n = recvfrom(sockfd, line, MAX_MESSAGE_SIZE - 1, 0, (struct sockaddr*)&cliaddr, &clilen)) < 0)
		{
			perror(NULL);
			close(sockfd);
			exit(1);
		}
		
		printf("%s -> ", user.nickname);
		printf("%s", line);
		
		if ( (k = is_private(line)) ) {
			for (i = 0; i < k; i++) 
			{
				nick[i] = line[i];
			}
			for (i = 0; i < MAX_MESSAGE_SIZE - k - 1; i++) 
			{
				line[i] = line[i + k + 1];
			}
			k = search_user(nick);
			if (k != -1) {
				create_sockaddr(&useraddr, users[k].ip.s_addr, users[k].port);
				send_message(line, user.nickname, useraddr);
			}
			else {
				send_message("You try to send message to non-existent user\n", "SERVER", cliaddr);
			}
		}
		else {
			send_to_all(line, user.nickname);
		}
		
	}
	return 0;
}
