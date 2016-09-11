#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void GenerateString(int n) {
	int i;
	char* string = (char*) calloc( (1 << n), sizeof(char));
	char *a = (char*)calloc( (1 << n), sizeof(char));
	char b[] = "a";
	a = strcat(a, string);
	for (i = 0; i < n; i++) 
	{
		a = strcat(a, b);
		a = strcat(a, string);
		string = strcpy(string, a);
		b[0] += 1;
	}
	printf("\n%s\n", a);
	free(string);
	free(a);
}

int main() {
	int n;
	scanf("%d", &n);
	GenerateString(n);
	return 0;
}
