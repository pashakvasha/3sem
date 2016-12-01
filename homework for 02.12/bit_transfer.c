#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define BITS_IN_BITE 8
#define MAX_RESULT_LENGTH 1000

int SIGUSR3 = 40;
pid_t pid;
pid_t ppid;
char *str = "Hello, world!@";
int length;
int current_bit;
char res[MAX_RESULT_LENGTH];

void send() {
	
	char bit;
	int symbol = current_bit / BITS_IN_BITE;
	int position = current_bit % BITS_IN_BITE;
	
	if (symbol == length)
		exit(0);

	bit = str[symbol] & (1 << position);
	
	current_bit++;
	
	if(bit)
		kill(pid, SIGUSR1);
	else
		kill(pid, SIGUSR2);
}

void receive(int nsig) {
	
	int symbol = current_bit / BITS_IN_BITE;
	int position = current_bit % BITS_IN_BITE;

	if (nsig == SIGUSR1)
		res[symbol] |= (1 << position);
	current_bit++;
	kill(ppid, SIGUSR3);
	if (current_bit / BITS_IN_BITE == length) {
		printf("%s\n", res);
		exit(0);
	}
}

int main() {
	length = strlen(str);

	ppid = getpid();
	pid = fork();
	
	if (pid != 0) {
		sleep(1);
		send();
		signal(SIGUSR3, send);
		while(1);
	} else {
		signal(SIGUSR1, receive);
		signal(SIGUSR2, receive);
		while(1);
	}
	
	return 0;
}
