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

	recv_msg(sockfd, buff, strlen(READY));
	if (strncmp(buff, READY, strlen(READY)) != 0)
		return;

	send_msg(sockfd, pass);

	recv_msg(sockfd, buff, strlen(OK));
	if (strncmp(buff, OK, strlen(OK)) != 0)
		return;

	recv_msg(sockfd, buff, strlen(MATRICULA));
	if (strncmp(buff, MATRICULA, strlen(MATRICULA)) != 0)
		return;

	char id[ID_LEN + 1];
	rand_num(id, ID_LEN);
	send_msg(sockfd, id);
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
