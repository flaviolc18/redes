#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include <utils.h>

void help(char *program, int argc, char *argv[], char *hint)
{
	printf("usage: %s ", program);
	for (int i = 0; i < argc; i++)
		printf("%s ", argv[i]);
	printf("\n");
	if (hint != NULL)
		printf("%s\n", hint);
	exit(EXIT_SUCCESS);
}

void logmsg(char *msg)
{
	printf("%s\n", msg);
}

void logexit(char *msg)
{
	logmsg(msg);
	exit(EXIT_FAILURE);
}

int send_msg(int fd, void *msg, int len)
{
	int sent_bytes = send(fd, msg, len, 0);
	if (sent_bytes != len)
	{
		perror("ERROR send()");
		close(fd);
		return FAIL;
	}
	return SUCCESS;
}

int send_str(int fd, char *str)
{
	return send_msg(fd, str, strlen(str));
}

int send_int(int fd, int num)
{
	int i = htonl(num);
	return send_msg(fd, &i, sizeof(int));
}

int recv_msg(int fd, void *buff, int len)
{
	int recvd_bytes = recv(fd, buff, len, MSG_WAITALL);

	if (recvd_bytes != len)
	{
		if (errno = EAGAIN || errno == EWOULDBLOCK)
			return TIMEOUT;

		perror("ERROR recv()");
		close(fd);
		return FAIL;
	}
	return SUCCESS;
}

int recv_str(int fd, char *str, int len)
{
	int rst = recv_msg(fd, str, len);
	str[len] = '\0';
	return rst;
}

int recv_int(int fd, int *num)
{
	int rst = recv_msg(fd, num, sizeof(int));
	*num = ntohl(*num);
	return rst;
}

void rand_str(char *str, int size)
{
	srand(time(0));

	char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	int len = sizeof(charset);

	for (int n = 0; n < size; n++)
		str[n] = charset[rand() % (len - 1)];
	str[size] = '\0';
}

int rand_int()
{
	srand(time(0));
	return rand();
}

void concat(char *dest, char *s1, int l1, char *s2, int l2)
{
	for (int i = 0; i < l1 + l2; i++)
		if (i < l1)
			dest[i] = s1[i];
		else
			dest[i] = s2[i - l1];
}