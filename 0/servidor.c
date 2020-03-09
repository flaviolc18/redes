#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>

#include <utils.h>
#include <lista.h>

#define PORT 51511
#define MAX_CON 10

int start_server(struct sockaddr_in *servaddr)
{
	int sockopt_rst, servsock = socket(AF_INET, SOCK_STREAM, 0);
	if (servsock == -1)
		logexit("ERROR socket()");

	sockopt_rst = setsockopt(servsock, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
	if (sockopt_rst == -1)
		logexit("ERROR setsockopt(SO_REUSEADDR)");

	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	sockopt_rst = setsockopt(servsock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	if (sockopt_rst == -1)
		logexit("ERROR setsockopt(SO_RCVTIMEO)");

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

void handler_prof(int clisock, struct list *students)
{
	struct node *it;

	for (it = begin(students); it != end(students); it = next(it))
	{
		char id[ID_LEN + 2];
		padln(it->val, 4, '0', id);
		send_msg(clisock, id);
	}

	send_msgl(clisock, END, 1);

	char buff[strlen(OK) + 1];
	recv_msg(clisock, buff, strlen(OK));
}

void handler_stu(int clisock, struct list *students)
{
	send_msg(clisock, OK);
	send_msg(clisock, MATRICULA);

	char id[ID_LEN + 1];
	recv_msg(clisock, id, ID_LEN);
	push(students, atoi(id));
}

void handler(int clisock, char *pass_prof, char *pass_stu, struct list *students)
{
	send_msg(clisock, READY);

	char pass[PASS_LEN + 1];
	recv_msg(clisock, pass, PASS_LEN);

	if (strncmp(pass, pass_prof, PASS_LEN) != 0)
		handler_prof(clisock, students);
	else if (strncmp(pass, pass_stu, PASS_LEN) != 0)
		handler_stu(clisock, students);
}

int main(int argc, char *argv[])
{

	if (argc < 3 || strlen(argv[1]) != 8 || strlen(argv[2]) != 8)
	{
		char *args[2] = {"<professor_pass>", "<student_pass>"};
		char *hint = "passwords must be 8 bytes long.";
		help(argv[0], 2, args, hint);
	}

	char *pass_professor = argv[1];
	char *pass_student = argv[2];

	struct sockaddr_in servaddr;
	int servsock = start_server(&servaddr);

	struct list students;
	init_list(&students);

	while (1)
	{
		struct sockaddr_in cliaddr;
		int cliaddr_len = sizeof(cliaddr);

		int clisock = accept(servsock, (struct sockaddr *)&cliaddr, &cliaddr_len);
		if (clisock == -1)
			continue;

		char cliaddr_p[INET_ADDRSTRLEN];
		handler(clisock, argv[1], argv[2], &students);
		close(clisock);
	}

	delete_list(&students);
}
