#ifndef ACCEPT_H
#define ACCEPT_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "reflect.h"

#define Server_IP			"127.0.0.1"
#define Server_PORT	 1234

class Accept : public Reflect
{
public:
	Accept();
	~Accept();

	void Start_listen_accept();
	void setnonblocking(int sock);

	int serv_sock;

};

#endif
