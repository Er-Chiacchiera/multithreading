#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int dim = 5;
	void * par =  &dim;


	int num = *((int*)par);
	//*(int*) par;
	printf("%d\n", num);
	return 0;





	char* stringa;
	char stringa [20];
}