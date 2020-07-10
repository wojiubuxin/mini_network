#include "accept.h"
Accept::Accept()
{
	
}

Accept::~Accept()
{
	close(serv_sock);
}

void Accept::Start_listen_accept()
{
	//创建socket
	serv_sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);

	//初始化addr
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(Server_IP);
	serv_addr.sin_port = htons(Server_PORT);

	//绑定
	bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	//监听
	listen(serv_sock, SOMAXCONN);

	//从反射的基对象里获取到其它类的内容
	OBJ_GET_EXT(m_Context, Epoll, ex_epoll);

	//循环接收
	int clnt_sock = 0;
	while (1)
	{
		struct sockaddr_in clnt_addr;
		socklen_t clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		DebugPrintf_2("新接收到的socket为%d", clnt_sock);
		if (clnt_sock >= 0)
		{
			setnonblocking(clnt_sock);
			struct epoll_event ev;
			ev.data.fd = clnt_sock;
			ev.events = EPOLLIN;
			ex_epoll->epoll_add(ex_epoll->epfd, clnt_sock, &ev);

		}

	}

}

void Accept::setnonblocking(int sock)
{
	int opts;
	opts = fcntl(sock, F_GETFL);
	if (opts >= 0)
	{
		opts = opts | O_NONBLOCK;
		fcntl(sock, F_SETFL, opts);
	}
}
