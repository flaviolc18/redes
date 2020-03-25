#ifndef POLL_H
#define POLL_H

#include <pthread.h>

struct poll
{
	pthread_t *threads;
	int length;
};

struct worker_args
{
	int id;
	struct poll *p;
	void *args;
	void *(*routine)(void *);
};

void init_poll(struct poll *, int);

int is_poll_full(struct poll *p);

void add_worker(struct poll *, void *(*)(void *), void *);

void delete_poll(struct poll *);

void join_workers(struct poll *);

#endif