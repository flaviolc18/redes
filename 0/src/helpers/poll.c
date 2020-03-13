#include <stdlib.h>
#include <pthread.h>

#include <poll.h>

void init_poll(struct poll *p)
{
	struct worker *head = (struct worker *)malloc(sizeof(struct worker));
	struct worker *tail = (struct worker *)malloc(sizeof(struct worker));

	head->prev = NULL;
	head->next = tail;
	tail->prev = head;
	tail->next = NULL;

	p->head = head;
	p->tail = tail;
}

void add_worker(struct poll *p, pthread_t id)
{
	struct worker *n = (struct worker *)malloc(sizeof(struct worker));
	n->id = id;

	n->prev = p->tail->prev;
	n->next = p->tail;

	p->tail->prev->next = n;
	p->tail->prev = n;
}

void join_workers(struct poll *p)
{
	for (struct worker *it = p->head->next; it != p->tail; it = it->next)
	{
		pthread_join(it->id, NULL);
	}
}

void delete_poll(struct poll *p)
{
	struct worker *it = p->head->next;
	while (it != p->tail)
	{
		it = it->next;
		free(it->prev);
	}
	free(p->head);
	free(p->tail);
}
