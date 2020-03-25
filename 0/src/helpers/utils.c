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

int recv_msg(int fd, void *buff, int len, int flag)
{
	int recvd_bytes = recv(fd, buff, len, flag);

	if (recvd_bytes != len && flag == MSG_WAITALL)
	{
		if (errno = EAGAIN || errno == EWOULDBLOCK)
			return TIMEOUT;

		close(fd);
		return FAIL;
	}
	return recvd_bytes;
}

int recv_str(int fd, char *str, int len)
{
	int rst = recv_msg(fd, str, len, MSG_WAITALL);
	str[len] = '\0';
	return rst;
}

int recv_int(int fd, int *num)
{
	int rst = recv_msg(fd, num, sizeof(int), MSG_WAITALL);
	*num = ntohl(*num);
	return rst;
}

void rand_str(char *str, int size)
{
	char charset[] = "0123456789";
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

int find(char *str, char c, int len)
{
	for (int i = 0; i < len; i++)
		if (str[i] == c)
			return i;
	return -1;
}

int msleep(long miliseconds)
{
	struct timespec req, rem;

	if (miliseconds > 999)
	{
		req.tv_sec = (int)(miliseconds / 1000);
		req.tv_nsec = (miliseconds - ((long)req.tv_sec * 1000)) * 1000000;
	}
	else
	{
		req.tv_sec = 0;
		req.tv_nsec = miliseconds * 1000000;
	}

	return nanosleep(&req, &rem);
}