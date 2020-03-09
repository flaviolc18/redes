#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <utils.h>
#include <cliente.h>

int connect_serv(struct sockaddr_in *servaddr)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		logexit("ERROR socket()");

	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	int sockopt_rst = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	if (sockopt_rst == -1)
		logexit("ERROR setsockopt(SO_RCVTIMEO)");

	servaddr->sin_family = AF_INET;
	inet_pton(AF_INET, SERV_IP, &servaddr->sin_addr);
	servaddr->sin_port = htons(PORT);

	int conn_rst = connect(sockfd, (struct sockaddr *)servaddr, sizeof(*servaddr));
	if (conn_rst != 0)
		logexit("ERROR connect()");

	return sockfd;
}

void check_args(int argc, char *argv[])
{
	if (argc < 2 || strlen(argv[1]) != 8)
	{
		char *args[1] = {"<password>"};
		char *hint = "password must be 8 bytes long.";
		help(argv[0], 1, args, hint);
	}
}

void checkexit(int code)
{
	if (code == TIMEOUT)
		return logexit("TIMEOUT");
	logexit("");
}
