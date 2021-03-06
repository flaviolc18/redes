#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <utils.h>
#include <cliente.h>

int connect_serv(struct sockaddr_in *servaddr, char *ip, char *port)
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
	inet_pton(AF_INET, ip, &servaddr->sin_addr);
	servaddr->sin_port = htons(atoi(port));

	int conn_rst = connect(sockfd, (struct sockaddr *)servaddr, sizeof(*servaddr));
	if (conn_rst != 0)
		logexit("ERROR connect()");

	return sockfd;
}

void check_args(int argc, char *argv[])
{
	if (argc < 4 || strlen(argv[3]) != 8)
	{
		char *args[3] = {"<IP>", "<port>", "<password>"};
		char *hint = "password must be 8 bytes long.";
		help(argv[0], 3, args, hint);
	}
}

void checkexit(int code)
{
	if (code == TIMEOUT)
		return logexit("TIMEOUT");
	logexit("");
}
