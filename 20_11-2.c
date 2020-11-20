#include <stdio.h>
#include <sys/inotify.h>
#include <unistd.h>

#define PATH_MAX 4096

int main(int argc, char ** argv){
	int in, watch, ret;
	int mask = IN_CREATE | IN_DELETE | IN_MOVE_SELF;
	char * buf[PATH_MAX];
	struct inotify_event* event;
	
	in = inotify_init();
	if (in < 0){
		perror("inotify_init error");
		return -1;
	}
	
	watch = inotify_add_watch(in, argv[1], mask);
	if (watch < 0){
		perror("inotify_add_watch error");
		return -1;
	}
	

	while(1){
		
		ret = read(in, buf, PATH_MAX);
		if(ret > 0){
			event = (struct inotify_event*) buf;
			printf("path: %s	", event->name);
			if(event->mask & IN_CREATE)
				printf("event: create\n");
			else if(event->mask & IN_DELETE)
				printf("event: delete\n");
			else if(event->mask & IN_MOVE_SELF)
				printf("event: move_self\n");
			
		} else {
			perror("read error");
			return -1;
		}
	}
	close(in);
	return 0;
}