#ifndef LIST_H
#define LIST_H

struct node
{
	unsigned int val;
	struct node *next;
	struct node *prev;
};

struct list
{
	struct node *head;
	struct node *tail;
};

void init_list(struct list *l);

int empty(struct list *l);

void push(struct list *l, int val);

int pop(struct list *l);

void delete_list(struct list *l);

struct node *begin(struct list *l);

struct node *end(struct list *l);

struct node *next(struct node *n);

#endif