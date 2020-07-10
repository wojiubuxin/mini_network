#ifndef EPOLL_H
#define EPOLL_H
#include <sys/epoll.h>
#include "accept.h"
#include <functional>
class Epoll : public Reflect
{
public:
	Epoll();
	~Epoll();

	void	Start_epoll();
	void handleEvent();

	void epoll_add(int fd, struct epoll_event* ev);
	void epoll_mod(int fd, struct epoll_event* ev);
	void epoll_del(int fd, struct epoll_event* ev);

	int epfd;
	int m_maxEventsSize;

	typedef std::vector<int> client_sock;
	client_sock m_elient;

	typedef std::vector<struct epoll_event> EpollFdList;
	EpollFdList m_epollEvents;

	void Read_Event(int sockfd);
	void Write_Event(int sockfd);
	void Close_Event(int sockfd);
};




#endif