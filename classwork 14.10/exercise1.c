#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>

#define STRING_SIZE 1000
#define STRING_COUNT 500

int main()
{
    char* array;
    FILE* src = NULL;
	src = fopen("exercise1.c", "r");
	
	if (src == NULL) {
	    perror("in.txt");  
	    return 7;          
	}
	
    int shmid;
    int new = 1;
    const char pathname[] = "exercise1.c";
    key_t   key;

    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if((shmid = shmget(key, (STRING_COUNT *STRING_SIZE) * sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if(errno != EEXIST){
            printf("Can't create shared memory\n");
            exit(-1);
        } else {
            if((shmid = shmget(key, (STRING_COUNT *STRING_SIZE) * sizeof(char), 0)) < 0) {
                printf("Can't find shared memory\n");
                exit(-1);
            }
            new = 0;
        }
    }

    if((array = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }
    
    char *s = calloc(STRING_SIZE, sizeof(char));
    s = fgets(s, STRING_SIZE, src);
    
    while (s != NULL) {
		strcat(array, s);
		s = fgets(s, STRING_SIZE, src);
	}

    if(shmdt(array) < 0){
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    
    free(s);
	fclose(src);
	return 0;
}
