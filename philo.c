#include "philo.h"

void	write_message(t_philo *philo, char *message)
{
	pthread_mutex_lock(&philo->write_lock);
	printf("%ld %d %s\n", get_time() - philo->start_time, philo->id, message);
	pthread_mutex_unlock(&philo->write_lock);
}

void	checkIsDead(t_philo *philo)
{
	int	i;
	int	cont;

	cont = 1;
	while (cont == 1)
	{
		i = 0;
		pthread_mutex_lock(&philo->data->check_lock);
		while (cont && (i < philo->data->philo_nbr))
		{
			if (get_time() - philo->last_meal_time > philo->data->time_to_die)
			{
				// printf("philo is %d\n", philo->id);
				philo->isDead = true;
				cont = 0;
				write_message(philo, "died");
				// return ;
				// exit(1);
			}
			// TODO: Check full or not if argument 6th exists
			i++;
		}
		pthread_mutex_unlock(&philo->data->check_lock);
	}
}

int	anyoneDied(t_philo *philo)
{
	int	ret;
	int	i;

	ret = 0;
	i = 0;
	pthread_mutex_lock(&philo->data->check_lock);
	while (i < philo->data->philo_nbr)
	{
		if (philo->data->philos[i].isDead == true)
			ret = 1;
		i++;
	}
	pthread_mutex_unlock(&philo->data->check_lock);
	return (ret);
}

void	waitForAction(t_philo *philo, t_status status)
{
	long	start;

	start = get_time();
	while (anyoneDied(philo) != true)
	{
		if (status == EATING)
		{
			if (get_time() - start >= philo->data->time_to_eat)
				return ;
		}
		else if (status == SLEEPING)
		{
			if (get_time() - start >= philo->data->time_to_sleep)
				return ;
		}
		ft_usleep(50);
	}
	return ;
}

void	*routine(void *data)
{
	t_philo *philo;

	philo = (t_philo *) data;
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
			waitForAction(philo, EATING);
			pthread_mutex_unlock(&philo->first_fork->fork);
			pthread_mutex_unlock(&philo->second_fork->fork);
		}

		if (anyoneDied(philo) != true)
		{
			write_message(philo, "is sleeping");
			waitForAction(philo, SLEEPING);
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
			ft_usleep(20);
		}
	}
	checkIsDead(data->philos);
	i = 0;
	while (i < data->philo_nbr)
	{
		pthread_join(data->philos[i].thread_id, NULL);
		// printf("Joining philo %d......\n", data->philos->id);
		i++;
	}
}
