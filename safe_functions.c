#include "philo.h"

void	*safe_malloc(size_t size)
{
	void	*ret;

	ret = malloc(size);
	if (ret == NULL)
		error_exit("Malloc error!");
	return (ret);
}

// void	safe_mutex_handler(t_mtx *mutex, t_ecode ecode)
// {
// 	if (ecode = LOCK)
// 		pthread_mutex_lock(mutex);
// 	else if (ecode = UNLOCK)
// 		pthread_mutex_unlock(mutex);
// 	else if (ecode = INIT)
// 		pthread_mutex_init(mutex, NULL);
// 	else if (ecode = DESTROY)
// 		pthread_mutex_destroy(mutex);
// }