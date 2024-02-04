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
	while (cont)
	{
		i = 0;
		while (i < philo->data->philo_nbr)
		{
			pthread_mutex_lock(&philo->data->check_lock);
			// printf("Philo %d\n", philo->id);
			if (get_time() - philo->last_meal_time > philo->data->time_to_die)
			{
				philo->isDead = true;
				cont = 0;
				write_message(philo, "died");
				// printf("Philo ID %d [%d]\n", philo->id, philo->isDead);
				return ;
			}
			// else if (philo->check_full == philo->num)
			// {
			// 	cont = 0;
			// 	philo->dead = 1;
			// }
			i++;
			pthread_mutex_unlock(&philo->data->check_lock);
		}
	}
	// if (get_time() - philo->last_meal_time > philo->data->time_to_die)
	// {
	// 	philo->isDead = true;
	// 	printf("last_meal_time is %ld | time to die is %ld\n", philo->last_meal_time, philo->data->time_to_die);
	// 	write_message(philo, "died");
	// 	break;
	// }
}

int	return_dead(t_philo *philo)
{
	int	ret;
	int	i;
	// int	j;

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

void	waitCheck(t_philo *philo, t_status status)
{
	long	start;

			// printf("Return_dead(philo %d) %d\n", philo->id, ret);
	start = get_time();
	while (return_dead(philo) != 1)
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
	while (return_dead(philo) != 1)
	{
		// printf("done done\n");
		// pthread_mutex_lock(&philo->debug);
		// printf("Philo[%d] entered\n", philo->id);
		// pthread_mutex_unlock(&philo->debug);
		if (return_dead(philo) != 1)
		{
			pthread_mutex_lock(&philo->first_fork->fork);
			write_message(philo, "has taken a fork");
			pthread_mutex_lock(&philo->second_fork->fork);
			write_message(philo, "has taken a fork");
			write_message(philo, "is eating");
			// TODO: CHECK REQUIRED NUMBER OF MEALS
			philo->last_meal_time = get_time();
			// pthread_mutex_lock(&philo->check_lock);
			// printf("Philosopher %d | last_meal_time: %ld | time_to_die: %ld\n",
			//        philo->id, get_time() - philo->last_meal_time, philo->data->time_to_die);
			// if (return_dead(philo) == 1)
			// 	break ;
			// printf("hi\n");
			// pthread_mutex_unlock(&philo->check_lock);
			waitCheck(philo, EATING);
			pthread_mutex_unlock(&philo->first_fork->fork);
			pthread_mutex_unlock(&philo->second_fork->fork);
		}
			// printf("Return_dead(philo %d) %d\n", philo->id, return_dead(philo));
		if (return_dead(philo) != 1)
		{
			write_message(philo, "is sleeping");
			waitCheck(philo, SLEEPING);
			write_message(philo, "is thinking");
			// printf("done here?\n");
		}
	}
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
		}
	}
	checkIsDead(data->philos);
	i = 0;
	while (i < data->philo_nbr)
	{
		printf("Joining philo [%d].....\n", i + 1);
		pthread_join(data->philos[i].thread_id, NULL);
		// pthread_detach(data->philos[i].thread_id);
		i++;
	}
}
