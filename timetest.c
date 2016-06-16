#include <stdio.h>
#include <time.h>

int main()
{
	time_t current_time;
	char *cp;	
	
	time(&current_time);
	cp = ctime(&current_time);
	printf("tims : %s\n", cp+4);	
	return 0;
}
