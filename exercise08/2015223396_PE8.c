#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#define READ 0
#define WRITE 1

typedef struct pinfo{
	FILE *file;
	pid_t pid;
	struct pinfo *next;
}pinofo;

static pinfo *plist = NULL;

int main()
{
	FILE *fp;
	char buf[BUFSIZ];

	fp = popen2("ls", "r");
	while(fgets(buf,BUFSIZ, fp) != NULL)
		fputs(buf,stdout);
}

FILE *popen2(const char *command, const char *mode)
{
	int fd[2];
	pinfo *cur, *old;

	if(mode[0] != 'r' && mode[0] != 'w')
	{
		errno = EINVAL;
		return NULL;
	}

	if(mode[1] != 0)
	{
		errno = EINVAL;
		return NULL;
	}

	if(pipe(fd))
		return NULL;

	cur = (pinfo*) malloc(sizeof(pinfo));
	if(!cur)
	{
		errno = ENOMEN;
		return NULL;
	}
	cur->pid = fork();
	switch(cur->pid){
		case -1:
			close(fd[0]);
			close(fd[1]);
			free(cur);
			return NULL;
		case 0:
			for(old = plist; old; old = old->next)
				close(fileno(old->file));
			if(mode[0] == 'r')
				dup2(fd[1], STDOUT_FILENO);
			else
				dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
			close(fd[1]);
			execl("/bin/sh", "sh", "-c", command, (char*) NULL);
			exit(1);
		default:
			if(mode[0] == 'r')
			{
				close(fd[0]);
				if(!(cur->file = fdopen(fd[0], mode)))
					close(fd[1]);
			}else{
				close(fd[0]);
				if(!(cur->file = fdopen(fd[1], mode)))
					close(fd[1]);
			}
			cur->next = plist;
			plist = cur;
	}
	return cur->file;
}
