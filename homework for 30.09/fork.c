#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	int i, N;
	scanf("%d", &N);
	int status[N];
	for (i = 0; i < N; i++)
	{
		status[i] = i;
		pid_t pid = fork();
		if (pid > 0) {
			printf("\ngetppid = %d\ngetpid = %d\n", getppid(), getpid());
			wait( &(status[i+1]) );
			printf("\nMy child (process = %d) has died\n", i);
			exit(status[i]);
		}
	}
	
	return 0;
}

