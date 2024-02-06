#include "philo.h"

void	write_message(t_philo *philo, char *message)
{
	// printf("inwritemsg\n");
	if (anyoneDied(philo) == true)
	{
		// printf("someone died\n");
		return ;
	}
	else
	{
		// printf("printmsg\n");
		pthread_mutex_lock(&philo->write_lock);
		printf("%ld %d %s\n", get_time() - philo->start_time, philo->id, message);
		pthread_mutex_unlock(&philo->write_lock);
	}
}

void	checkIsDead(t_philo *philo)
{
	int	i;
	int	cont;
	long time_to_die;
	int	nbrOfPhilo;

	cont = 1;
	pthread_mutex_lock(&philo->data->check_lock);
	time_to_die = philo->data->time_to_die;
	nbrOfPhilo = philo->data->philo_nbr;
	pthread_mutex_unlock(&philo->data->check_lock);
	while (cont == 1)
	{
		i = 0;
		while (cont && (i < nbrOfPhilo))
		{
			if (get_time() - philo->last_meal_time > time_to_die)
			{
				// printf("philo is %d\n", philo->id);
				write_message(philo, "died");
				pthread_mutex_lock(&philo->data->check_lock);
				philo->isDead = true;
				pthread_mutex_unlock(&philo->data->check_lock);
				cont = 0;
			}
			// TODO: Check full or not if argument 6th exists
			i++;
		}
	}
}

int	anyoneDied(t_philo *philo)
{
	int	ret;
	int	i;
	int	n;

	ret = 0;
	i = 0;
	pthread_mutex_lock(&philo->data->check_lock);
	n = philo->data->philo_nbr;
	pthread_mutex_unlock(&philo->data->check_lock);
	while (i < n)
	{
		pthread_mutex_lock(&philo->data->check_lock);
		if (philo->data->philos[i].isDead == true)
			ret = 1;
		pthread_mutex_unlock(&philo->data->check_lock);
		i++;
	}
	return (ret);
}

void	waitForAction(t_philo *philo, long time)
{
	long	start;
	// long	eat;
	// long	sleep;

	start = get_time();
	// pthread_mutex_lock(&philo->data->check2_lock);
	// eat = philo->data->time_to_eat;
	// sleep = philo->data->time_to_sleep;
	// pthread_mutex_unlock(&philo->data->check2_lock);
	while (anyoneDied(philo) != true)
	{
		if (get_time() - start >= time)
			return ;
		ft_usleep(20);
	}
	return ;
}

// void	waitForAction(t_philo *philo, long time)
// {
// 	long	start;
// 	// long	eat;
// 	// long	sleep;

// 	start = get_time();
// 	// pthread_mutex_lock(&philo->data->check2_lock);
// 	// eat = philo->data->time_to_eat;
// 	// sleep = philo->data->time_to_sleep;
// 	// pthread_mutex_unlock(&philo->data->check2_lock);
// 	while (anyoneDied(philo) != true)
// 	{
// 		if (status == EATING)
// 		{
// 			if (get_time() - start >= eat)
// 				return ;
// 		}
// 		else if (status == SLEEPING)
// 		{
// 			if (get_time() - start >= sleep)
// 				return ;
// 		}
// 		ft_usleep(50);
// 	}
// 	return ;
// }

void	*routine(void *philoPassed)
{
	t_philo *philo;
	long	eat;
	long	sleep;
	
	// pthread_mutex_lock(&((t_philo *) philoPassed)->data->check_lock);
	philo = (t_philo *) philoPassed;
	// pthread_mutex_unlock(&philo->data->check_lock);
	pthread_mutex_lock(&philo->data->check_lock);
	eat = philo->data->time_to_eat;
	sleep = philo->data->time_to_sleep;
	pthread_mutex_unlock(&philo->data->check_lock);
	while (anyoneDied(philo) != true)
	{
		if (anyoneDied(philo) != true)
		{
			// pthread_mutex_lock(&philo->debug);
			// printf("still here??\n");
			// pthread_mutex_unlock(&philo->debug);
			pthread_mutex_lock(&philo->first_fork->fork);
			write_message(philo, "has taken a fork");
			pthread_mutex_lock(&philo->second_fork->fork);
			write_message(philo, "has taken a fork");
			write_message(philo, "is eating");
			philo->last_meal_time = get_time();
			// TODO: CHECK REQUIRED NUMBER OF MEALS
			// pthread_mutex_lock(&philo->check_lock);
			// printf("hi\n");
			// pthread_mutex_unlock(&philo->check_lock);
			waitForAction(philo, eat);
			pthread_mutex_unlock(&philo->first_fork->fork);
			pthread_mutex_unlock(&philo->second_fork->fork);
		}

		if (anyoneDied(philo) != true)
		{
			write_message(philo, "is sleeping");
			waitForAction(philo, sleep);
			write_message(philo, "is thinking");
			// printf("done here?\n");
		}
		// printf("Philo %d done!\n", philo->id);
	}
	// printf("Philo %d exited!\n", philo->id);
	return (NULL);
}

void	start_routine(t_data *data)
{
	int	i;

	i = 0;
	if (data->meals_nbr == 0)
		return ;
	else if (data->meals_nbr == 1)
		; //TODO
	else
	{
		while (i < data->philo_nbr)
		{
			// printf("i is %d\n", i);
			pthread_create(&data->philos[i].thread_id, NULL, &routine, &data->philos[i]);
			i++;
			usleep(100);
		}
	}
	// printf("heree\n");
	checkIsDead(data->philos);
	i = 0;
	while (i < data->philo_nbr)
	{
		pthread_join(data->philos[i].thread_id, NULL);
		// printf("Joining philo %d......\n", data->philos->id);
		i++;
	}
}
