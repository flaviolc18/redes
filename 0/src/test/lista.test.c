#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <lista.h>

#define END "\0"

int main()
{
	struct list l;
	init_list(&l);

	push(&l, 1);
	push(&l, 2);
	push(&l, 3);
	push(&l, 4);

	printf("%d\n", pop(&l));
	printf("%d\n", pop(&l));
	printf("%d\n", pop(&l));

	delete_list(&l);

	return 0;
}
