#ifndef UTILS_H
#define UTILS_H

#define MAX_LEN 1024
#define FAIL 0
#define SUCCESS 1
#define TIMEOUT 2
#define READY "READY"
#define OK "OK"
#define MATRICULA "MATRICULA"
#define PASS_LEN 8
#define ID_LEN 4
#define END "\0"

void help(char *program, int argc, char *argv[], char *hint);

void logmsg(char *msg);

void logexit(char *msg);

int send_msg(int fd, void *msg);

// send message passing len as param (msgl = msg len)
int send_msgl(int fd, void *msg, int len);

int recv_msg(int fd, char *buff, int len);

// receive raw message, no '\0' at the end (msgr = msg raw)
int recv_msgr(int fd, char *buff, int len);

void rand_str(char *str, int size);

void rand_num(char *num, int size);

void padln(int n, int size, char c, char *dest);

void concat(char *dest, char *s1, int l1, char *s2, int l2);

#endif