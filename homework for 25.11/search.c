#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_PATH_SIZE 1000
#define MAX_FILENAME_SIZE 100

char * final_path;
char * filename;
int is_found;
struct stat buf;

void find_DFS(char * path, int depth) {
	if (depth < 0)
		return;
	//printf("current path = %s\n", path);
	DIR * direct = opendir(path);
	struct dirent* dirent;
	while ( (dirent = readdir(direct)) != NULL )
	{
/*
 * У вас следующие три строки встречаются несколько раз в коде.
 * Их стоит вынести в отдельную ф-ю append_to_path или вроде того.
 */
		char * name = calloc(MAX_PATH_SIZE, sizeof(char));
		name = strcat(name, path);
		name = strcat(name, dirent->d_name);
		stat(name, &buf);
		if (S_ISREG(buf.st_mode)) {
			//printf("file = %s\n", dirent->d_name);
			if ( !strcmp(dirent->d_name,filename) ) {
				printf("FILE HAS BEEN FOUND\n");
				is_found = 1;
				strcpy(final_path, path);
				free(name);
				closedir(direct);
				return;
			}
		}
		free(name);
	}
	rewinddir(direct);
	while ( ((dirent = readdir(direct)) != NULL) && ( !is_found ) )
	{
		char * name = calloc(MAX_PATH_SIZE, sizeof(char));
		name = strcat(name, path);
		name = strcat(name, dirent->d_name);
		stat(name, &buf);
		if ( (S_ISDIR(buf.st_mode)) && ( strcmp(dirent->d_name, "..") ) && ( strcmp(dirent->d_name, ".") ) ) {
			//printf("directory = %s\n", dirent->d_name);
			char * next_path = calloc(MAX_PATH_SIZE, sizeof(char) );
			strcat(next_path, path);
			strcat(next_path, dirent->d_name);
			strcat(next_path, "/");
			//printf("next path = %s\n", next_path);
			find_DFS( next_path, depth - 1 );
			free(next_path);
		}
		free(name);
	}
	closedir(direct);
}


int main(int argc, char* argv[]) {
	char * start_path = calloc( MAX_PATH_SIZE, sizeof(char) );
	start_path = strcat(start_path, argv[1]);
	
	filename = calloc(MAX_FILENAME_SIZE, sizeof(char) );
	filename = strcat(filename, argv[2]);
	
	final_path = calloc( MAX_PATH_SIZE, sizeof(char) );
	
	find_DFS(start_path, atoi(argv[3]) );
	
	if (is_found) {
		printf("%s\n", strcat(final_path, filename) );
	} else {
		printf("FILE HASN'T BEEN FOUND!\n");
	}
	
	free(start_path);
	free(final_path);
	free(filename);
	return 0;
}
