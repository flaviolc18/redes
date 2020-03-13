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
	recv_msg(sockfd, buff, 5);
	if (strncmp(buff, READY, 5) != 0)
		return;

	send_msg(sockfd, pass);

	while (1)
	{
		char fb[1], rest[ID_LEN], id[ID_LEN];
		int rst_recv;

		rst_recv = recv_msgr(sockfd, fb, 1);
		if (rst_recv != SUCCESS)
			return checkexit(rst_recv);

		if (strncmp(fb, END, 1) != 0)
		{
			rst_recv = recv_msgr(sockfd, rest, ID_LEN);
			if (rst_recv != SUCCESS)
				return checkexit(rst_recv);

			concat(id, fb, 1, rest, ID_LEN - 1);
			printf("%d\n", ntohl(*(unsigned int *)id));
		}
		else
			break;
	}

	send_msg(sockfd, OK);
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
