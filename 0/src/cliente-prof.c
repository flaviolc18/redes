#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#include <utils.h>
#include <cliente.h>

void handler(int sockfd, char *pass)
{
	int rst_recv;
	char buff[MAX_LEN];
	rst_recv = recv_str(sockfd, buff, strlen(READY));
	if (rst_recv <= 0)
		return checkexit(rst_recv);

	if (strncmp(buff, READY, strlen(READY)) != 0)
		return;

	send_str(sockfd, pass);

	while (1)
	{
		rst_recv = recv_msg(sockfd, buff, MAX_LEN - 1, 0);
		if (rst_recv <= 0)
			return checkexit(rst_recv);

		if (find(buff, END, rst_recv) == -1)
		{
			buff[rst_recv] = END;
			printf("%s", buff);
		}
		else
		{
			printf("%s", buff);
			break;
		}
	}

	send_str(sockfd, OK);
}

int main(int argc, char *argv[])
{
	check_args(argc, argv);

	struct sockaddr_in servaddr;
	int sockfd = connect_serv(&servaddr, argv[1], argv[2]);

	handler(sockfd, argv[3]);
	close(sockfd);

	return 0;
}
