#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char ** argv){
	DIR *dir;
struct dirent *ent;
for(int i = 1; i < argc; i++){
	if ((dir = opendir (argv[i])) != NULL) {
		printf ("%s\n", argv[i]);
	  while ((ent = readdir (dir)) != NULL) {
		printf ("	%s\n", ent->d_name);
	  }
	  closedir (dir);
	} else {
	  printf("ERROR\n");
	}
}
	
	return 0;
}