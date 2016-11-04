#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define TABLE_LIMIT 5
#define BUFFER_SIZE 5

struct sembuf mybuf;
int semid;
int table;
char* profession[] = {"washer.txt", "dryer.txt"};

void sem(int param);
void create_sem(char* pathname);

int main(int argc, char* argv[]) {
	
	int profession_id = atoi(argv[1]);
	char pathname[] = "readme.txt";
	
	create_sem(pathname);
	
	const char fifo[] = "table.fifo";
	
	if ( (mknod(fifo , S_IFIFO | 0666 , 0) < 0) && (errno != EEXIST) ) {
		printf("Can`t create FIFO\n");
		exit(-1);
	}
	
	if ( atoi(argv[1]) == 0) {
		table = open(fifo , O_WRONLY);
	} else {
		table = open(fifo , O_RDONLY);
	}

    FILE* src = NULL;
	src = fopen(profession[profession_id], "r");
	
	if (src == NULL) {
	    perror("in.txt");  
	    return 7;          
	}
	
	int max_number = 0;
	int number, speed;
	
	while ( fscanf(src, "%d:%d\n", &number, &speed) == 2) 
	{
		if (number > max_number) {
			max_number = number;
		}
	}
	
	rewind(src);
	
	int *profession_speed = malloc(max_number * sizeof(int) );
	
	while ( fscanf(src, "%d:%d\n", &number, &speed) == 2) 
	{
		profession_speed[number] = speed;
	}
	
	fclose(src);
	
	int dish_type;
	char buffer[BUFFER_SIZE];
	
	if (profession_id == 0) {
		
		int amount, i;
		
		sem(TABLE_LIMIT);
		
		FILE* dishes = NULL;
		dishes = fopen("dishes.txt", "r");
	
		if (dishes == NULL) {
			perror("in.txt");  
			return 7;          
		}
		while (1)
		{
			fscanf(dishes, "%d:%d", &dish_type, &amount);
			if (dish_type == 0) {
				sprintf(buffer, "%d\n", dish_type);
				write(table, buffer, BUFFER_SIZE);
				printf("WASHER HAS FINISHED HIS WORK\n");
				printf("Now there are %d dishes on the table.\n", TABLE_LIMIT - semctl(semid, 0, GETVAL));
				break;
			}
			for (i = 0; i < amount; i++)
			{
				sleep(profession_speed[dish_type]);
				printf("Washer has washed the dish %d and put it on the table.\n", dish_type);
				sprintf(buffer, "%d\n", dish_type);
				write(table, buffer, BUFFER_SIZE);
				sem(-1);
				printf("Now there are %d dishes on the table\n", TABLE_LIMIT - semctl(semid, 0, GETVAL));
			}
		}
		
		fclose(dishes);
	} else {
		while (1) 
		{
			read(table, buffer, BUFFER_SIZE);
			dish_type = atoi(buffer);
			if (dish_type == 0) {
				printf("DRYER HAS FINISHED HIS WORK\n");
				printf("Now there are %d dishes on the table.\n", TABLE_LIMIT - semctl(semid, 0, GETVAL));
				break;
			}
			sleep(profession_speed[dish_type]);
			printf("Dryer has dried the dish %d and put it away from the table.\n", dish_type);
			sem(1);
			printf("Now there are %d dishes on the table.\n", TABLE_LIMIT - semctl(semid, 0, GETVAL));
		}
	}
	close(table);
	
	if (profession_id == 1) {
		semctl(semid, 0, IPC_RMID, 0);
	}
	
	return 0;
}

void sem(int param) {
	mybuf.sem_flg = 0;
    mybuf.sem_num = 0;
	mybuf.sem_op = param;
	if (semop(semid , &mybuf , 1) < 0) {
		printf("Can`t wait for condition\n");
		exit(-1);
	}
}

void create_sem(char* pathname) {
	key_t key;
	
	if ( (key = ftok(pathname , 0)) < 0 ) {
        printf("Can`t generate key\n");
        exit(-1);
    }

    if ((semid = semget(key , 1 , 0666 | IPC_CREAT)) < 0) {
        printf("Can`t get semid\n");
        exit(-1);
    }
}
