#include <stdio.h>

int main(int ac, char *av[])
{
	if(ac == 2){
		printf("a number of ac : %d\n",ac);
		printf("first : %s and second : %s\n", *av, *av+1);
	}
	return 0;
}
