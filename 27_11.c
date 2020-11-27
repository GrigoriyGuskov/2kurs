#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SIZE 1024

int main(int argc, char ** argv){
	
	if( argc < 2 ){
		printf("Too few arguments. Check the entered data.\n");
		exit(-1);
	}
	
	int nfds = argc - 1;
	int * files = (int*) calloc(nfds , sizeof(int));
	
	for (int i = 1; i < argc; ++i)
	{
		files[i - 1] = mkfifo(argv[i], O_CREAT | 0666);
		if (files[i - 1] == -1)
		{
			printf("error mkfifo %s\n", argv[i]);
		}
	}
	int count = 0;
	for(int i = 1; i < argc; i++){
		files [i - 1]= open(argv[i],O_NONBLOCK | O_RDONLY);
		if(files [i - 1] <= 0){
			printf("error opening %s\n", argv[i]);
			count++;
		}
	}
	if (count == nfds){
		printf("All files were not opened \n");
		exit(-1);
		}
		
	
	struct pollfd * polls = calloc(nfds , sizeof(struct pollfd));
	
	for (int i = 0; i < nfds; i++) {
		polls[i].fd = files[i];
		polls[i].events = POLLIN;
	}
	
	char* buf =  calloc(SIZE , sizeof(char));
	
	 while ( poll(polls, nfds, -1) != -1) {

		for (int i = 0; i < nfds; i++) {
			int R = read(files[i], buf, SIZE);
			if (R == -1) {
				printf("error reading %s\n", argv[i+1]);
				R = 0;
			}
			else if (R != 0) { 
				int W = write(STDOUT_FILENO, buf, R);
				if (W == -1) 
					printf("error writing\n");
			}
		}
	}
	
	free(files);
	free(polls);
	free(buf);
	return 0;
}