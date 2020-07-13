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
	std::queue<task> task_list;					  //�������
	bool isstart;										//�߳̿�ʼ��־
	int thread_num;								  //�߳���
	pthread_mutex_t m_pthreadmutex; //ͬ����
	pthread_cond_t	m_pthreadcond;   //ͬ������

	ThreadPool();
	virtual ~ThreadPool();
	void *ThreadFunc();							 //�̲߳�������
	void Create_ThreadPool();				 //�����̳߳�
	void AddTask(task ctask);				 //�������
	void StopAll_Thread();						 //�˳������߳�
	int GetTaskSize();							     //��ȡ������
	task get_task();									//��ȡ����
};


#endif