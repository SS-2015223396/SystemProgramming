#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utmp.h>

#define BUFF_SIZE 1024
int main()
{
	char buff[BUFF_SIZE];
	int file_descriptor = open("who3.c", O_RDONLY);
	printf("open return value : %d\n", file_descriptor);
	
	int filesize = sizeof("who3.c");
	int readsize = read(file_descriptor, buff, BUFF_SIZE);
	printf("read return value : %d\n", readsize);
	
	close("who3.c");	
	return 0;
}
