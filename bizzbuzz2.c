#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>



int main(int argc, char ** argv){
	
	if( argc < 3 ){
		printf("Too few arguments. Check the entered data.\n");
		exit(-1);
	}
	if( !strcmp(argv[1] , argv[2]) ){
		printf("Same file names. Check the entered data.\n");
		exit(-1);
	}
	struct stat info1;
	int first = open(argv[1],O_RDONLY);
		
	switch (first) {
		case EACCES:
			printf("Запрошенный доступ к файлу не разрешен.\n");
			exit(-1);
		case EROFS:
			printf("Файл предназначен только для чтения.\n");
			exit(-1);
		case EFAULT:
			printf("Pathname указывает на каталог за пределами доступного адресного пространства.\n");
			exit(-1);
		case ENOTDIR:
			printf("Компонент, который обозначен как каталог в pathname, таковым не является.\n");
			exit(-1);
		default:
			break;
	}
	
	int second = open(argv[2],O_RDWR | O_CREAT | O_TRUNC, 0666);
	
	switch (second) {
		case EACCES:
			printf("Запрошенный доступ к файлу не разрешен.\n");
			exit(-1);
		case EROFS:
			printf("Файл предназначен только для чтения.\n");
			exit(-1);
		case EFAULT:
			printf("Pathname указывает на каталог за пределами доступного адресного пространства.\n");
			exit(-1);
		case ENOTDIR:
			printf("Компонент, который обозначен как каталог в pathname, таковым не является.\n");
			exit(-1);
		default:
			break;
	}

	if (fstat(first, &info1)){
		printf("error\n");
		exit(-1);
	}

	unsigned int size = info1.st_size;
	char * buf = calloc(size, sizeof(char));
	char * buf0 = buf;
	size = read (first, buf, size);
	unsigned int size0 = size;
	//printf("%s\n", buf);					//isdigit	isspace
	char * buf2 = calloc(size, sizeof(char));
	int i = 0;
	int cflag = 0;
	int rflag = 0;
	int j = 0;
	int last = 9;
	int sum = 0;
	while (buf[0]){
		sum = 0;

		for(j = 0; !isspace(buf[j]) && buf[j] != '\0'; j++){
			rflag = 1;
			if (isdigit(buf[j]) && !cflag) {
				last = buf[j] - '0';
				sum += last;
			}
			else 
				cflag = 1;
		}
		if (!cflag && rflag){
			if (sum % 3 == 0){
				if( last % 5 == 0) {
					size = size + 8 -j;
					buf2 = realloc (buf2, size);
					strcpy(buf2 + i, "bizzbuzz");
					i += 8;
					strncpy ( buf2 + i, buf + j, 1);
					i ++;
				}
				else {
					size = size + 4 -j;
					buf2 = realloc (buf2, size);
					strcpy(buf2 + i, "bizz");
					i += 4;
					strncpy ( buf2 + i, buf + j, 1);
					i ++;
				}
			}
			else if ( last % 5 == 0) {
				size = size + 4 -j;
				buf2 = realloc (buf2, size);
				strcpy(buf2 + i, "buzz");
				i += 4;
				strncpy ( buf2 + i, buf + j, 1);
				i ++;
				}
			else {
				strncpy ( buf2 + i, buf, j + 1);
				i += j + 1;
			}
		}
		else {
			strncpy ( buf2 + i, buf, j + 1);
			i += j + 1;
		}
		buf += j + 1;
		if (buf - buf0 > size0 )
			buf = buf0 + size0 + 1 ;

		cflag = 0;
		rflag = 0;
	}
		
	
	write (second, buf2, size);
	
	free(buf0);
	free(buf2);
	
	close(first);
	close(second);
	
	return 0;
}