#include "threadpool.h"
#include <sys/sysinfo.h>

ThreadPool::ThreadPool()
{
	isstart = true;
	thread_num = 1;
	m_pthreadmutex = PTHREAD_MUTEX_INITALIZER;
	m_pthreadcond = PTHREAD_COND_INITALIZER;

}

ThreadPool::~ThreadPool()
{
	isstart = false;
	StopAll_Thread();
}

void ThreadPool::Create_ThreadPool()
{
	thread_num = get_nprocs() * 2;
	pthread_id = (pthread_t)malloc(sizeof(pthread_t)*thread_num);
	for (int i = 0; i < thread_num; i++)
	{
		pthread_create(&pthread_id[i], NULL, ThreadFunc, NULL);
	}
}

void ThreadPool::StopAll_Thread()
{
	pthread_cond_broadcast(&m_pthreadcond);	//唤醒所有线程

	for (int i = 0; i < thread_num; i++)
	{
		pthread_join(pthread_id[i], NULL);
	}

	free(pthread_id);
	pthread_id = NULL;

	pthread_mutex_destroy(&m_pthreadmutex);
	pthread_cond_destroy(&m_pthreadcond);
}

void * ThreadPool::ThreadFunc()
{
	while (isstart)
	{
		task ctask = get_task();
		ctask.run(ctask.arg);

	}
}

task ThreadPool::get_task()
{
	zhuge_mutex temp(m_pthreadmutex);
	while (task_list.empty())
	{
		pthread_cond_wait(&m_pthreadcond, &m_pthreadmutex);
	}
	task front = task_list.front();
	task_list.pop();
	return front;
}
int ThreadPool::GetTaskSize()
{
	zhuge_mutex temp(m_pthreadmutex);
	return task_list.size();
}

void ThreadPool::AddTask(task ctask)
{
	zhuge_mutex temp(m_pthreadmutex);
	task_list.push(ctask);
	pthread_cond_signal(&m_pthreadcond);

}




