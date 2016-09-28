#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_STRING_SIZE 1000
#define MAX_TOKEN_SIZE 100
#define MAX_NUMBER_OF_TOKENS 100

void Split(char * s, char * delim, char*** tokens, int* count);
void MyGets(char **s, FILE* src);


int main(int argc, char const *argv[])
{
	int i = 0, tokensCount = 0;
	char delim[1] = {' '};
	char* s = calloc(MAX_STRING_SIZE, sizeof(char) );
	FILE* src = NULL;
	src = fopen("test.txt", "r");
	
	if (src == NULL) {
	    perror("in.txt");  
	    return 7;          
	}

	char **tokens = malloc(MAX_NUMBER_OF_TOKENS * sizeof(char*) );

	for (i = 0; i < MAX_NUMBER_OF_TOKENS; i++) 
	{
		tokens[i] = calloc(MAX_TOKEN_SIZE, sizeof(char) );
	}
	
	MyGets(&s, src);
	
	int stringsCount = atoi(s);
	int status[stringsCount];
	
	for (i = 0; i < stringsCount; i++)
	{
		MyGets(&s, src);
		status[i] = i;
		pid_t pid = fork();
		if (pid == 0)
		{
			Split(s, delim, &tokens, &tokensCount);
			sleep(atoi(tokens[0]));
			execvp(tokens[1], tokens + 1);
			exit(status[i]);
		}
	}
	
	for (i = 0; i < stringsCount; i++)
	{
		wait( &(status[i]) );
	}
	
	for (i = 0; i < MAX_NUMBER_OF_TOKENS; i++) 
	{
		free(tokens[i]);
	}
	free(tokens);
	free(s);
	fclose(src);
	
	return 0;
}

void Split(char * s, char * delim, char*** tokens, int* count) {
	if (s == NULL)
		return;
	char* token = strtok(s, delim);
	while (token != NULL)
	{
		strcpy( (*tokens)[*count], token);
		(*count)++;
		token = strtok(NULL, delim);
	}
	(*tokens)[*count] = NULL;
}

void MyGets(char **s, FILE* src) {
	int c, i = 0;
	while ( ( (c = fgetc(src)) != '\n' ) && (i < MAX_STRING_SIZE - 1) ) {
		(*s)[i] = c;
		i++;
	}
	(*s)[i] = '\0';
}
