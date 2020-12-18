
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


int pipe_count(char* s);
void close_all(int* arr, int count);
void palki(char* command, int* arr, int count, int num);
void bez_palok(char* buf);

int main(int argc, char** argv) {

	int fd;
	if( argc < 2 ){
		printf("Too few arguments. Check the entered data.\n");
		exit(-1);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		printf("opening error\n");
		exit(1);
	}  
	struct stat info1;
	fstat(fd, &info1);
	int size = info1.st_size;
	char * buf = malloc(size);
	int rd = read(fd, buf, size - 1 	);
	if ( rd < 0 ) {
		printf("error reading\n");
		exit(1);
	}
	int count = pipe_count(buf) + 1;
	if ( count == 1 ) {
	  bez_palok(buf);
	}
	int arr[count * 2];
	for(int i = 0; i < count; i++) {
		pipe((int*)arr + i * 2);
	}
	char* buf2 = (char*)malloc(rd);
	strcpy(buf2, buf);
	for(int i = 0; i < count; i++) {
		char* command = strtok_r(buf2, "|", &buf2);
		pid_t kid = fork();
		if ( kid == 0 ) {
			palki(command, (int*) arr, count, i);
		}
	}
	close_all((int*) arr, count);
	for(int i = 0; i < count + 1; ++i) {
		wait(NULL);
	}
	free(buf);
	return 0;
}

int pipe_count(char* s) {
	int count = 0;
	int len = strlen(s);
	for(int i = 0; i < len; i++) {
		if ( s[i] == '|' ) {
			count++;
		}
	}
	return count;
}

void close_all(int* arr, int count) {
	
	for(int i = 0; i < count*2; i++) {
		close(arr[i]);
	}
}

void palki(char* command, int* arr, int count, int num) {
	int len = strlen(command);
	char **action = (char**)malloc(len);
	int i = 0;
	while( (action[i] = strtok_r(command, " ", &command)) ) {
		i += 1;
	}
	if (num != 0) {
		dup2(arr[(num - 1) * 2], STDIN_FILENO);
	}
	if (num < (count - 1)) {
		dup2(arr[num * 2 + 1], STDOUT_FILENO);
	}
	close_all(arr, count);
	execvp(action[0], action);

	exit(1);
}

void bez_palok(char* buf) {
	int size = strlen(buf);
	char **action = (char**)malloc(size);
	int i = 0;
	while( (action[i] = strtok_r(buf, " ", &buf)) ) {
		i += 1;
	}
	execvp(action[0], action);
	
	free(action);
	exit(1);
	
}
