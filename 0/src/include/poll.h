#ifndef POLL_H
#define POLL_H

#include <pthread.h>

struct worker
{
	pthread_t id;
	struct worker *next;
	struct worker *prev;
};

struct poll
{
	struct worker *head;
	struct worker *tail;
};

void init_poll(struct poll *p);

void add_worker(struct poll *p, pthread_t id);

void delete_poll(struct poll *p);

void join_workers(struct poll *p);

#endif