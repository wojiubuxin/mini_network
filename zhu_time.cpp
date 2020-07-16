#include "zhu_time.h"

#define evutil_timercmp(tvp,uvp,cmp)				\
	(((tvp)->tv_sec == (uvp)->tv_sec) ?		\
	(((tvp)->tv_usec) cmp=(uvp)->tv_usec) :	\
	(((tvp)->tv_sec) cmp=(uvp)->tv_sec)			\
	)

ZhuTimer::ZhuTimer()
{
	timer_efd = epoll_create(1024);//大于0即可
	isstart = true;
}

ZhuTimer::~ZhuTimer()
{
	isstart = false;
	close(timer_efd);
	timer_event.clear();
}

void ZhuTimer::SetTimer(int interval, void(*fun)(void*), void *arg, int flag, int timeid)
{
	DebugPrintf_2("设置定时器毫秒数为%d\n", interval);
	timer_struct ev;
	struct timeval now;
	gettimeofday(&now, NULL);

	ev.ev_interval.tv_sec = interval / 1000;
	ev.ev_interval.tv_usec = (interval % 1000) * 1000;

	ev.ev_timeout.tv_sec = ev.ev_interval.tv_sec + now.tv_sec;
	ev.ev_timeout.tv_usec = ev.ev_interval.tv_usec + now.tv_usec;
	if (ev.ev_timeout.tv_usec >= 1000000)
	{
		ev.ev_timeout.tv_sec++;
		ev.ev_timeout.tv_usec -= 1000000;
	}

	ev.ev_callback = fun;
	ev.ev_arg = arg;
	ev.one_or_again = flag;
	ev.timer_id = timeid;
	timer_event.push_back(ev);

}

void ZhuTimer::Start_Timer_run()
{
	DebugPrintf_2("开始执行定时器");
	struct timeval now;
	struct epoll_event temp[1];//瞎写即可
	//精度选择
	int timer_out_epfd = 100;//100毫秒
	while (isstart)
	{
		epoll_wait(timer_efd, temp, 1, timer_out_epfd);
		if (timer_event.size() > 0)
		{
			gettimeofday(&now, NULL);

			for (std::vector<timer>::iterator it; it != timer_event.end();)
			{
				//如果当前时间大于之前设置的超时时间了,那么得执行
				if (evutil_timercmp(&now, &it->ev_timeout, >))
				{
					it->ev_callback(it->ev_arg);
					if (it->one_or_again == 0)
					{
						it = timer_event.erase(it);
						//这里不能++

					}//本来可以直接else的，之所以再加个if是为了后续有扩充其它类型的定时器打算，例如带次数的那种
					else if (it->one_or_again == 1)	
					{
						//重设时间
						it->ev_timeout.tv_sec = now.tv_sec + it->ev_interval.tv_sec;
						it->ev_timeout.tv_usec = now.tv_usec + it->ev_interval.tv_usec;
						if (it->ev_timeout.tv_usec >= 1000000)
						{
							it->ev_timeout.tv_sec++;
							it->ev_timeout.tv_usec -= 1000000;
						}
						//要放这里++
						it++;
					}

				}
			}



		}




	}


}
