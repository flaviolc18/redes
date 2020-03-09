#ifndef CLIENT_H
#define CLIENT_H

#define SERV_IP "127.0.0.1"
#define PORT 51511

int connect_serv(struct sockaddr_in *servaddr);

void check_args(int argc, char *argv[]);

#endif