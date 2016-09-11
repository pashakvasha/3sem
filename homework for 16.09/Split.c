#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Split(char * s, char * delim, char*** tokens, int* count);
void MyGets(char **s);


int main() {
	int i = 0, count = 0;
	
	char *s = calloc(1000, sizeof(char) );
	MyGets(&s);
	
	
	char* delim = calloc(10, sizeof(char) );
	MyGets(&delim);
	
	char **tokens = malloc(100 * sizeof(char*) );
	
	for (i = 0; i < 100; i++) 
	{
		tokens[i] = malloc(100 * sizeof(char) );
	}
	
	Split(s, delim, &tokens, &count);
	
	for (i = 0; i < count; i++) {
		printf("tokens[%d] = %s\n", i, tokens[i]);
	}
	
	for (i = 0; i < 100; i++) 
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
	while ( (c = getchar()) != '\n') {
		(*s)[i] = c;
		i++;
	}
	s[i + 1] = '\0';
}
