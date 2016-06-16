#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define oops(m,x) { perror(m); exit(1); }

int main(int ac, char *argv[])
{
	int thepipe[2];
	int newfd;
	int pid;

	if(ac != 4)
	{
		fprintf(stderr, "usage: pipe cmd1 cmd2 cmd3 \n");
		exit(1);
	}
	if(pipe(thepipe) == -1)
		oops("Cannot get a pipe", 1);
	if((pid = fork()) == -1)
		oops("Cannot fork", 2);
	if(pid != 0)
	{
		close(thepipe[1]);
		if(dup2(thepipe[0],0) == -1)
			oops("Could not redirect stdin", 3);
		close(thepipe[0]);
		execlp(argv[2], argv[2], NULL);
		oops(argv[2], 4);
	}
	else
	{
		if((pid = fork()) == -1)
			oops("Cannot fork", 4);
		if(pid != 0)
		{
			close(thepipe[1]);
			if(dup2(thepipe[0], 0) == -1)
				oops("Could not redirect stdin",4);
			close(thepipe[0]);
			execlp(argv[3], argv[3], NULL);
			oops(argv[3], 5); 
		}
		close(thepipe[0]);
		if(dup2(thepipe[1], 1) == -1)
			oops("Could not redirect stdout", 5);
		close(thepipe[1]);
		execlp(argv[1], argv[1], NULL);
		oops(argv[1], 5);
	}
	return 0;
}
