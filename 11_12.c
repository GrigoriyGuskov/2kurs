#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int main(int argc, char** argv) {
	
    if( argc < 2 ){
		printf("Too few arguments. Check the entered data.\n");
		exit(-1);
	}
	
    struct flock f1 = {0};
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf("opening error\n");
        exit(1);
    }  
    int ret;
    f1.l_type = F_RDLCK;
    f1.l_whence = SEEK_CUR;
    while(1){
		if ((ret = fcntl(fd, F_GETLK, &f1)) >= 0 ) {
			printf("file name %s\n", argv[1]);
			printf("type %d\n", f1.l_type);
			printf("whence %d\n", f1.l_whence);
			printf("start %ld\n", f1.l_start);
			printf("length %ld\n", f1.l_len);
			printf("pid %d\n", f1.l_pid);
		} else {
			printf("no more locks\n");
			close(fd);
			exit(-1);
		}
	}
    return 0;
}