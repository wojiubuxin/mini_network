#ifndef ZHU_TIME_H
#define ZHU_TIME_H
#include "Reflect.h"
#include "zhu_head.h"
#include <sys/epoll.h>
typedef struct timer_struct
{
	struct timerval ev_interval;			//当初设置的时间
	struct timerval ev_timeout;			//计算到期的时间

	void(*ev_callback)(void *arg);		//回调函数,用std::function也可绑定类的成员函数
	void *ev_arg;

	int timer_id;							//事件ID,业务上时可随意定具体跑哪个函数
	int one_or_again;					//0代表只执行一次就删掉,1表示无限执行下去，不剔除, 但要重新刷下时间

}timer;

class ZhuTimer : public Reflect
{
public:
	ZhuTimer();
	~ZhuTimer();

	void Start_Timer_run();
	

	//第一个代表毫秒级,例如1秒=1000毫秒，如要定时1秒，传1000即可
	//fun=回调函数
	//arg=参数，默认为空
	//flag=代表是一次还是无限次定时器，默认为0
	//timerid=默认为0，业务专用的
	void SetTimer(int interval, void(*fun)(void*), void *arg=NULL, int flag=0, int timeid=0);



	int timer_efd;				//此epoll_id为定时器独用
	bool isstart;
	std::vector<timer> timer_event;

};

#endif
