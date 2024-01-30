#include "philo.h"

void	*routine(void *data)
{
	t_philo *philo;

	philo = (t_philo *) data;
	while (philo->data->dead == false)
	{
		long	start;

		start = get_time();
		printf("%ld %d is thinking\n", get_time() - start, philo->id);
		
		pthread_mutex_lock(&philo->first_fork->fork);
		printf("%ld %d has taken a fork\n", get_time() - start, philo->id);
		// pthread_mutex_unlock(&philo->first_fork->fork);
		pthread_mutex_lock(&philo->second_fork->fork);
		printf("%ld %d has taken a fork\n", get_time() - start, philo->id);
		printf("%ld %d is eating\n", get_time() - start, philo->id);
		ft_usleep(philo->data->time_to_eat);

		// pthread_mutex_unlock(&philo->second_fork->fork);
		// philo->data->dead = true;
	}

	return (NULL);
}

void	start_simulation(t_data *data)
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
		i = 0;
		while (i < data->philo_nbr)
		{
			pthread_join(data->philos[i].thread_id, NULL);
			i++;
		}
	}
}
