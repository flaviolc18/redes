#ifndef UTILS_H
#define UTILS_H

#define MAX_LEN 1024
#define SUCCESS 1
#define FAIL -1
#define TIMEOUT -2
#define READY "READY"
#define OK "OK"
#define MATRICULA "MATRICULA"
#define PASS_LEN 8
#define ID_LEN 4
#define END '\0'
#define ENDL '\n'

void help(char *program, int argc, char *argv[], char *hint);

void logmsg(char *msg);

void logexit(char *msg);

int send_msg(int fd, void *msg, int len);

int send_str(int fd, char *str);

int send_int(int fd, int num);

int recv_msg(int fd, void *buff, int len, int flag);

int recv_str(int fd, char *str, int len);

int recv_int(int fd, int *num);

void rand_str(char *str, int size);

int rand_int();

void concat(char *dest, char *s1, int l1, char *s2, int l2);

int find(char *str, char c, int len);

int msleep(long miliseconds);

#endif