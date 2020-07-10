#include "epoll.h"
Epoll::Epoll()
{
	epfd = epoll_create(1024);//大于0即可

}

Epoll::~Epoll()
{
	for (client_sock::iterator it = m_elient.begin(); it != m_elient.end(); it++)
	{
		Close_Event(*it);
	}
	m_elient.clear();
	close(epfd);

}

void Epoll::Start_epoll()
{
	m_maxEventsSize = 0;
	m_epollEvents.reserve(1024);

	int numEvents = 0;
	while (1)
	{
		numEvents = epoll_wait(epfd, m_epollEvents.data(), m_maxEventsSize, 0);
		if (numEvents > 0)
		{
			handleEvent();
		}
	}
}

void Epoll::handleEvent()
{
	for (EpollFdList::iterator it = m_epollEvents.begin(); it != m_epollEvents.end(); it++)
	{
		if (it->events&EPOLLIN)
		{
			Read_Event(it->data.fd);
		}
		else if (it->events&EPOLLOUT)
		{
			Write_Event(it->data.fd);
		}
		else
		{
			Close_Event(it->data.fd);
		}
	}
	m_epollEvents.clear();
}

void Epoll::Read_Event(int sockfd)
{
	char buf[BUFSIZ];//这里不考虑处理粘包问题，倘若需要，可自定义个包的长度安插在前面，或是自定义结构体
	int flag = read(sockfd, buf, sizeof(buf));
	char str[1024];
	if (flag > 0)
	{
		sprintf(str, "客户端Socket号为%d, 收到的内容为%s\n", sockfd, buf);
		printf(str);
		DebugPrintf_2(str);

		strcat(str, "=ACK");
		printf(str);
		DebugPrintf_2(str);
		write(sockfd, buf, sizeof(buf));
	}
	else
	{
		int sockErr = errno;
		if (flag == -1 && (sockErr == EAGAIN || sockErr == EINTR))
		{
			//直接跳过，不需断开socket
			return;
		}

		sprintf(str, "客户端Socket号为%d, flag=%d, 出现异常%d，需断开socket\n", sockfd, flag, sockErr);
		printf(str);
		DebugPrintf_2(str);

		//此处应断开
		Close_Event(sockfd);
	}


}

void Epoll::Write_Event(int sockfd)
{
	char buf[BUFSIZ];
	sprintf(buf, "服务端发给客户端Socket=%d, FUCK!\n", sockfd);
	printf(buf);
	DebugPrintf_2(buf);
	write(sockfd, buf, sizeof(buf));

}

void Epoll::Close_Event(int sockfd)
{
	char buf[BUFSIZ];
	sprintf(buf, "关闭客户端Socket=%d\n", sockfd);
	printf(buf);
	DebugPrintf_2(buf);
	epoll_del(sockfd, NULL);//直接NULL即可
}


void Epoll::epoll_add(int fd, struct epoll_event* ev)
{
	int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, ev);
	if (ret < 0)
	{
		DebugPrintf_2("添加epoll_add错误ret=%d, fd=%d", ret, fd);
	}
	m_maxEventsSize++;
	m_elient.push_back(fd);
}

void Epoll::epoll_mod(int fd, struct epoll_event* ev)
{
	int ret = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, ev);
	if (ret < 0)
	{
		DebugPrintf_2("修改epoll_mod错误ret=%d, fd=%d", ret, fd);
	}
}

void Epoll::epoll_del(int fd, struct epoll_event* ev)
{
	int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, ev);
	if (ret < 0)
	{
		DebugPrintf_2("删除epoll_del错误ret=%d, fd=%d", ret, fd);
	}
	close(fd);
}
