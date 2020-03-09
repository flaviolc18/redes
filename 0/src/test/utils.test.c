#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <utils.h>

int main()
{
	char rst[5];
	padln(323, 4, '0', rst);
	printf("%s\n", rst);

	// char *s = "\0";
	// if (strncmp(s, END, 1) == 0)
	// 	printf("IGUAIS\n");
	// else
	// 	printf("DIFF\n");

	// printf("%d\n", strlen(s));
	return 0;
}