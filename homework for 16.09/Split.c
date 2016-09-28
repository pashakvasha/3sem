#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE 1000
#define MAX_TOKEN_SIZE 100
#define NUMBER_OF_TOKENS 100

void Split(char * s, char * delim, char*** tokens, int* count);
void MyGets(char **s);

int main() {
	int i = 0, count = 0;
	
	char *s = calloc(MAX_STRING_SIZE, sizeof(char) );
	MyGets(&s);
	
	
	char* delim = calloc(MAX_STRING_SIZE, sizeof(char) );
	MyGets(&delim);
	
	char **tokens = malloc(NUMBER_OF_TOKENS * sizeof(char*) );
	
	for (i = 0; i < NUMBER_OF_TOKENS; i++) 
	{
		tokens[i] = malloc(MAX_TOKEN_SIZE * sizeof(char) );
	}
	
	Split(s, delim, &tokens, &count);
	
	for (i = 0; i < count; i++) {
		printf("tokens[%d] = %s\n", i, tokens[i]);
	}
	
	for (i = 0; i < NUMBER_OF_TOKENS; i++) 
	{
		free(tokens[i]);
	}
	free(tokens);
	free(delim);
	free(s);
	return 0;
}

void Split(char * s, char * delim, char*** tokens, int* count) {
	char* token = strtok(s, delim);
	while (token != NULL)
	{
		strcpy( (*tokens)[*count], token);
		(*count)++;
		token = strtok(NULL, delim);
	}
}

void MyGets(char **s) {
	int c, i = 0;
	while ( ( (c = getchar()) != '\n' ) && (i < MAX_STRING_SIZE - 1) ) {
		(*s)[i] = c;
		i++;
	}
	(*s)[i] = '\0';
}
