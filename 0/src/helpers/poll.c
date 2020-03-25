#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include <poll.h>
#include <utils.h>

void init_poll(struct poll *p, int length)
{
	p->threads = (pthread_t *)malloc(sizeof(pthread_t) * length);
	p->length = length;

	for (int i = 0; i < p->length; i++)
		p->threads[i] = -1;
}

int get_free_worker(struct poll *p)
{
	for (int i = 0; i < p->length; i++)
		if (p->threads[i] == -1)
			return i;
	return -1;
}

int is_poll_full(struct poll *p)
{
	return get_free_worker(p) == -1;
}

void *worker(void *args)
{
	struct worker_args *wa = (struct worker_args *)args;

	wa->routine(wa->args);

	wa->p->threads[wa->id] = -1;
	free(args);

	return NULL;
}

void add_worker(struct poll *p, void *(*routine)(void *), void *args)
{
	int id = get_free_worker(p);
	if (id == -1)
		return;

	struct worker_args *wa = malloc(sizeof(struct worker_args));
	wa->p = p;
	wa->id = id;
	wa->args = args;
	wa->routine = routine;

	int pthread_rst = pthread_create(&(wa->p->threads[wa->id]), NULL, worker, (void *)wa);
	if (pthread_rst != 0)
	{
		logmsg("ERROR pthread_create");
		return;
	}
}

void join_workers(struct poll *p)
{
	for (int i = 0; i < p->length; i++)
		if (p->threads[i] != -1)
			pthread_join(p->threads[i], NULL);
}

void delete_poll(struct poll *p)
{
	free(p->threads);
}
