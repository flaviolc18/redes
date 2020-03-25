#ifndef CLIENT_H
#define CLIENT_H

int connect_serv(struct sockaddr_in *servaddr, char *ip, char *port);

void check_args(int argc, char *argv[]);

void checkexit(int code);

#endif