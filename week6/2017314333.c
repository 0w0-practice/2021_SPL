#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	unsigned int pid;
	time_t t;
	struct tm *tm;	
	int fd;
	char *argv[3];
	char buf[512];
	int fd0, fd1, fd2;

	fd = open("./crontab", O_RDWR);
	pid = fork();
	
	if(pid == -1) return -1;
	if(pid != 0)
		exit(0);
	if(setsid() < 0)
		exit(0);
	if(chdir("/") < 0)
		exit(0);

	umask(0);

	close(0);
	close(1);
	close(2);

	fd0 = open("/dev/null", O_RDWR);
	fd1 = open("/dev/null", O_RDWR);
	fd2 = open("/dev/null", O_RDWR);

	t = time(NULL);
	tm = localtime(&t);
	
	read(fd, buf, sizeof(buf));
	close(fd);

	while (1)
	{
		//buf[0] = '\0';	

		// insert your code

		// ##  hints  ##

		// strtok_r();
		// pid = fork();
		// execl("/bin/sh", "/bin/sh", "-c", argv[2], (char*) NULL);
		
		
		char* pos=buf;
		char* token;
		int i=0;
		while((token=strtok_r(pos, " ", &pos))){
			argv[i++]=token;
		}
		

		t = time(NULL);
		tm = localtime(&t);

		if(((atoi(argv[0])==tm->tm_min)||(argv[0][0]=='*'))&&((atoi(argv[1])==tm->tm_hour)||argv[1][0]=='*')){
			pid=fork();
			if(pid==0){
				execl("/bin/sh", "/bin/sh", "-c", argv[2], (char*)NULL);
				break;
			}else{
				wait(NULL);
			}
		}
			
	
		sleep(60 - tm->tm_sec % 60);
	}

	return 0;
}
