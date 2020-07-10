#include "epoll.h"
Epoll::Epoll()
{
	epfd = epoll_create(1024);//����0����

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
	char buf[BUFSIZ];//���ﲻ���Ǵ���ճ�����⣬������Ҫ�����Զ�������ĳ��Ȱ�����ǰ�棬�����Զ���ṹ��
	int flag = read(sockfd, buf, sizeof(buf));
	char str[1024];
	if (flag > 0)
	{
		sprintf(str, "�ͻ���Socket��Ϊ%d, �յ�������Ϊ%s\n", sockfd, buf);
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
			//ֱ������������Ͽ�socket
			return;
		}

		sprintf(str, "�ͻ���Socket��Ϊ%d, flag=%d, �����쳣%d����Ͽ�socket\n", sockfd, flag, sockErr);
		printf(str);
		DebugPrintf_2(str);

		//�˴�Ӧ�Ͽ�
		Close_Event(sockfd);
	}


}

void Epoll::Write_Event(int sockfd)
{
	char buf[BUFSIZ];
	sprintf(buf, "����˷����ͻ���Socket=%d, FUCK!\n", sockfd);
	printf(buf);
	DebugPrintf_2(buf);
	write(sockfd, buf, sizeof(buf));

}

void Epoll::Close_Event(int sockfd)
{
	char buf[BUFSIZ];
	sprintf(buf, "�رտͻ���Socket=%d\n", sockfd);
	printf(buf);
	DebugPrintf_2(buf);
	epoll_del(sockfd, NULL);//ֱ��NULL����
}


void Epoll::epoll_add(int fd, struct epoll_event* ev)
{
	int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, ev);
	if (ret < 0)
	{
		DebugPrintf_2("���epoll_add����ret=%d, fd=%d", ret, fd);
	}
	m_maxEventsSize++;
	m_elient.push_back(fd);
}

void Epoll::epoll_mod(int fd, struct epoll_event* ev)
{
	int ret = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, ev);
	if (ret < 0)
	{
		DebugPrintf_2("�޸�epoll_mod����ret=%d, fd=%d", ret, fd);
	}
}

void Epoll::epoll_del(int fd, struct epoll_event* ev)
{
	int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, ev);
	if (ret < 0)
	{
		DebugPrintf_2("ɾ��epoll_del����ret=%d, fd=%d", ret, fd);
	}
	close(fd);
}
