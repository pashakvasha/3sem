#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct User {
	struct in_addr ip;
	unsigned short port;
	char* nickname;
} User;

User users[10];
char line[1000];
char nick[100];

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

User sign_up(unsigned short port, struct in_addr ip, char * nickname) {
	User user;
	int k = search_user(nickname);
	if (k != -1) {
		user = users[k];
	}
	else {
		users[last_user].nickname = calloc(100, sizeof(char));
		users[last_user].nickname = strcpy(users[last_user].nickname, nickname);
		users[last_user].ip = ip;
		users[last_user].port = port;
		user = users[last_user];
		printf("%s HAS SIGNED UP\n", users[last_user].nickname);
		last_user++;
	}
	return user;
}

void send_message(char * message, struct sockaddr_in recipient) {
	if (sendto(sockfd, message, strlen(message) + 1, 0, (struct sockaddr*)&recipient, sizeof(recipient)) < 0) {
		perror(NULL);
		close(sockfd);
		exit(1);
	}
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
		if ((n = recvfrom(sockfd, line, 999, 0, (struct sockaddr*)&cliaddr, &clilen)) < 0)
		{
			perror(NULL);
			close(sockfd);
			exit(1);
		}
		
		User user = sign_up(cliaddr.sin_port, cliaddr.sin_addr, line);
		
		if ((n = recvfrom(sockfd, line, 999, 0, (struct sockaddr*)&cliaddr, &clilen)) < 0)
		{
			perror(NULL);
			close(sockfd);
			exit(1);
		}
		
		printf("%s -> ", user.nickname);
		printf("%s", line);
		
		if ( (k = is_private(line)) ) {
			for (i = 0; i < k; i++) {
				nick[i] = line[i];
			}
			k = search_user(nick);
			create_sockaddr(&useraddr, users[k].ip.s_addr, users[k].port);
			send_message(user.nickname, useraddr);
			send_message(line, useraddr);
		}
		else {
			for (i = 0; i < last_user; i++) {
				create_sockaddr(&useraddr, users[i].ip.s_addr, users[i].port);
				send_message(user.nickname, useraddr);
				send_message(line, useraddr);
			}
		}
		
	}
	return 0;
}
