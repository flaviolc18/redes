#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
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

void log(char *msg)
{
	printf("%s\n", msg);
}

void logexit(char *msg)
{
	log(msg);
	exit(EXIT_FAILURE);
}

int send_msgl(int fd, void *msg, int len)
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

int send_msg(int fd, void *msg)
{
	int len = strlen(msg);
	return send_msgl(fd, msg, len);
}

int recv_msgr(int fd, char *buff, int len)
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

int recv_msg(int fd, char *buff, int len)
{
	int rst = recv_msgr(fd, buff, len);
	buff[len] = '\0';
	return rst;
}

static void rand_gen(char *str, int size, char *charset)
{
	srand(time(0));
	int len = sizeof charset;

	for (int n = 0; n < size; n++)
		str[n] = charset[rand() % (len - 1)];
	str[size] = '\0';
}

void rand_str(char *str, int size)
{
	char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	rand_gen(str, size, charset);
}

void rand_num(char *num, int size)
{
	char charset[] = "0123456789";
	rand_gen(num, size, charset);
}

void padln(int n, int size, char c, char *dest)
{
	char tmp[size];
	sprintf(tmp, "%d", n);

	int len = strlen(tmp);
	int diff = size - len;

	for (int i = 0; i < size; i++)
		if (i < diff)
			dest[i] = c;
		else
			dest[i] = tmp[i - diff];

	dest[size] = '\n';
}

void concat(char *dest, char *s1, int l1, char *s2, int l2)
{
	for (int i = 0; i < l1 + l2; i++)
		if (i < l1)
			dest[i] = s1[i];
		else
			dest[i] = s2[i - l1];
}