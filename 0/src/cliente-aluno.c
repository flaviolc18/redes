#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <utils.h>
#include <cliente.h>

void handler(int sockfd, char *pass)
{
	char buff[MAX_LEN];
	int rst_recv;

	rst_recv = recv_msg(sockfd, buff, strlen(READY));
	if (rst_recv != SUCCESS)
		return checkexit(rst_recv);

	if (strncmp(buff, READY, strlen(READY)) != 0)
		return;

	send_msg(sockfd, pass);

	rst_recv = recv_msg(sockfd, buff, strlen(OK));
	if (rst_recv != SUCCESS)
		return checkexit(rst_recv);

	if (strncmp(buff, OK, strlen(OK)) != 0)
		return;

	rst_recv = recv_msg(sockfd, buff, strlen(MATRICULA));
	if (rst_recv != SUCCESS)
		return checkexit(rst_recv);

	if (strncmp(buff, MATRICULA, strlen(MATRICULA)) != 0)
		return;

	unsigned int id = htonl(rand_int());
	send_msg(sockfd, &id);
}

int main(int argc, char *argv[])
{
	check_args(argc, argv);

	struct sockaddr_in servaddr;
	int sockfd = connect_serv(&servaddr);

	handler(sockfd, argv[1]);
	close(sockfd);

	return 0;
}
