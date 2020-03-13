#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <pthread.h>
#include <signal.h>

#include <utils.h>
#include <lista.h>
#include <poll.h>

#define PORT 51511
#define MAX_CON 10

pthread_mutex_t lock;
struct list students;
struct poll threads;

void close_server()
{
	join_workers(&threads);
	pthread_mutex_destroy(&lock);

	delete_poll(&threads);
	delete_list(&students);

	exit(EXIT_SUCCESS);
}

int start_server(struct sockaddr_in *servaddr)
{
	int sockopt_rst, servsock = socket(AF_INET, SOCK_STREAM, 0);
	if (servsock == -1)
		logexit("ERROR socket()");

	sockopt_rst = setsockopt(servsock, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
	if (sockopt_rst == -1)
		logexit("ERROR setsockopt(SO_REUSEADDR)");

	servaddr->sin_family = AF_INET;
	servaddr->sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr->sin_port = htons(PORT);

	int bind_rst = bind(servsock, (struct sockaddr *)servaddr, sizeof(*servaddr));
	if (bind_rst != 0)
		logexit("ERROR bind()");

	int listen_rst = listen(servsock, MAX_CON);
	if (listen_rst != 0)
		logexit("ERROR listen()");

	return servsock;
}

void check(int code)
{
	if (code == TIMEOUT)
		return logmsg("TIMEOUT");
}

void handler_prof(int clisock, struct list *students)
{
	pthread_mutex_lock(&lock);
	struct node *it;
	for (it = begin(students); it != end(students); it = next(it))
	{
		send_int(clisock, it->val);
		send_msg(clisock, ENDL, 1);
	}
	pthread_mutex_unlock(&lock);

	send_msg(clisock, END, 1);

	char buff[strlen(OK) + 1];
	int rst_recv = recv_str(clisock, buff, strlen(OK));
	if (rst_recv != SUCCESS)
		return check(rst_recv);
}

void handler_stu(int clisock, struct list *students)
{
	send_str(clisock, OK);
	send_str(clisock, MATRICULA);

	int id;
	int rst_recv = recv_int(clisock, &id);
	if (rst_recv != SUCCESS)
		return check(rst_recv);

	pthread_mutex_lock(&lock);
	push(students, id);
	pthread_mutex_unlock(&lock);
}

struct handler_args
{
	int clisock;
	char *pass_prof;
	char *pass_stu;
	struct list *students;
};

void *handler(void *args)
{
	struct handler_args *ha = (struct handler_args *)args;

	send_str(ha->clisock, READY);

	char pass[PASS_LEN + 1];
	int rst_recv = recv_str(ha->clisock, pass, PASS_LEN);
	if (rst_recv != SUCCESS)
	{
		check(rst_recv);
		return NULL;
	}

	if (strncmp(pass, ha->pass_prof, PASS_LEN) != 0)
		handler_prof(ha->clisock, ha->students);
	else if (strncmp(pass, ha->pass_stu, PASS_LEN) != 0)
		handler_stu(ha->clisock, ha->students);

	close(ha->clisock);
	return NULL;
}

int main(int argc, char *argv[])
{

	// for the sake of testing
	if (argc < 3 || strlen(argv[1]) != 8 || strlen(argv[2]) != 8)
	{
		char *args[2] = {"<professor_pass>", "<student_pass>"};
		char *hint = "passwords must be 8 bytes long.";
		help(argv[0], 2, args, hint);
	}

	// remove comment when submiting
	// char pass_professor[PASS_LEN + 1], pass_student[PASS_LEN + 1];
	// rand_str(pass_professor, PASS_LEN);
	// rand_str(pass_student, PASS_LEN);

	struct sockaddr_in servaddr;
	int servsock = start_server(&servaddr);

	init_list(&students);
	init_poll(&threads);

	signal(SIGINT, &close_server);

	int mutex_init_rst = pthread_mutex_init(&lock, NULL);
	if (mutex_init_rst != 0)
		logexit("ERROR pthread_mutex_init");

	while (1)
	{
		struct sockaddr_in cliaddr;
		socklen_t cliaddr_len = sizeof(cliaddr);

		int clisock = accept(servsock, (struct sockaddr *)&cliaddr, &cliaddr_len);
		if (clisock == -1)
			continue;

		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		int sockopt_rst = setsockopt(clisock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
		if (sockopt_rst == -1)
		{
			logmsg("ERROR setsockopt(SO_RCVTIMEO)");
			continue;
		}

		struct handler_args ha;
		ha.clisock = clisock;
		ha.pass_prof = argv[1];
		ha.pass_stu = argv[2];
		ha.students = &students;

		pthread_t ti;
		int pthread_rst = pthread_create(&ti, NULL, handler, (void *)&ha);
		if (pthread_rst != 0)
		{
			logmsg("ERROR pthread_create");
			continue;
		}
		add_worker(&threads, ti);
	}
}
