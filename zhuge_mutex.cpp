#include "zhuge_mutex.h"

zhuge_mutex::zhuge_mutex(pthread_mutex_t &mutex) :temp_mutex(mutex)
{
	pthread_mutex_lock(&temp_mutex);
}
zhuge_mutex::~zhuge_mutex()
{
	pthread_mutex_unlock(&temp_mutex);
}
