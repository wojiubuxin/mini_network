#ifndef ZHUGE_MUTEX_H
#define ZHUGE_MUTEX_H

#include "zhu_head.h"

class  zhuge_mutex
{
public:
	zhuge_mutex(pthread_mutex_t &mutex);
	~ zhuge_mutex();

	pthread_mutex_t temp_mutex;

};


#endif