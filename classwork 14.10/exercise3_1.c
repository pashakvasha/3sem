#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>

#define STRING "Paul is very good!"

struct sembuf mybuf;
int semid;

void sem(int param) {
	mybuf.sem_flg = 0;
    mybuf.sem_num = 0;
	mybuf.sem_op = param;
	if (semop(semid , &mybuf , 1) < 0) {
		printf("Can`t wait for condition\n");
		exit(-1);
	}
}

int main()
{
    FILE* src = NULL;
	src = fopen("print.txt", "a");
	
	if (src == NULL) {
	    perror("in.txt");  
	    return 7;          
	}

    const char pathname[] = "test.txt";

    key_t key;

    

    if ( (key = ftok(pathname , 0)) < 0 ) {
        printf("Can`t generate key\n");
        exit(-1);
    }

    if ((semid = semget(key , 1 , 0666 | IPC_CREAT)) < 0) {
        printf("Can`t get semid\n");
        exit(-1);
    }
    
    sem(1);
    
    while (1)
    {
		sem(-1);
		fprintf(src, STRING);
		sem(1);
	}

    
	fclose(src);
	
	return 0;
}
