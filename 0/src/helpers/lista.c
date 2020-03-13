#include <stdlib.h>

#include <lista.h>

void init_list(struct list *l)
{
	struct node *head = (struct node *)malloc(sizeof(struct node));
	struct node *tail = (struct node *)malloc(sizeof(struct node));

	head->prev = NULL;
	head->next = tail;
	tail->prev = head;
	tail->next = NULL;

	l->head = head;
	l->tail = tail;
}

int empty(struct list *l)
{
	return l->head == l->tail->prev;
};

void push(struct list *l, int val)
{
	struct node *n = (struct node *)malloc(sizeof(struct node));
	n->val = val;

	n->prev = l->tail->prev;
	n->next = l->tail;

	l->tail->prev->next = n;
	l->tail->prev = n;
}

void delete_list(struct list *l)
{
	struct node *it = l->head->next;
	while (it != l->tail)
	{
		it = it->next;
		free(it->prev);
	}
	free(l->head);
	free(l->tail);
}

struct node *begin(struct list *l)
{
	return l->head->next;
}

struct node *end(struct list *l)
{
	return l->tail;
}

struct node *next(struct node *n)
{
	if (n == NULL)
		return n;
	return n->next;
}
