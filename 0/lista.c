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

int pop(struct list *l)
{
	if (empty(l))
		// meus coração doe ao fazer isso
		return;

	struct node *n = l->tail->prev;
	int val = n->val;

	l->tail->prev->prev->next = l->tail;
	l->tail->prev = l->tail->prev->prev;

	free(n);
	return val;
}

void delete_list(struct list *l)
{
	struct node *it = l->head;
	while (it->next != NULL)
	{
		it = it->next;
		free(it->prev);
	}
	free(it);
}

struct node *begin(struct list *l)
{
	if (empty(l))
		return NULL;
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
