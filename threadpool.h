#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
#include "reflect.h"
#include <queue>
#include <pthread.h>
#include "zhuge_mutex.h"

struct task
{
	void (*run)(void *args);
	void *arg;
};

class ThreadPool : public Reflect
{
public:
	pthread_t		*pthread_id;
	std::queue<task> task_list;					  //任务队列
	bool isstart;										//线程开始标志
	int thread_num;								  //线程数
	pthread_mutex_t m_pthreadmutex; //同步锁
	pthread_cond_t	m_pthreadcond;   //同步变量

	ThreadPool();
	virtual ~ThreadPool();
	void *ThreadFunc();							 //线程操作函数
	void Create_ThreadPool();				 //创建线程池
	void AddTask(task ctask);				 //添加任务
	void StopAll_Thread();						 //退出所有线程
	int GetTaskSize();							     //获取任务数
	task get_task();									//获取任务
};


#endif