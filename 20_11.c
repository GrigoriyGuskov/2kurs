#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char ** argv){
	DIR *dir;
struct dirent *ent;
if ((dir = opendir (argv[1])) != NULL) {
  while ((ent = readdir (dir)) != NULL) {
    printf ("%s\n", ent->d_name);
  }
  closedir (dir);
} else {
  printf("ERROR\n");
}
	
	return 0;
}