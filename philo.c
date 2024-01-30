#include "philo.h"

void	write_message(t_philo *philo, char *message, int id)
{
	pthread_mutex_lock(&philo->write_lock);
	printf("%ld %d %s\n", get_time() - philo->start_time, id, message);
	pthread_mutex_unlock(&philo->write_lock);
}

void	*routine(void *data)
{
	t_philo *philo;

	philo = (t_philo *) data;
	while (philo->data->dead == false)
	{
		write_message(philo, "is thinking", philo->id);
		pthread_mutex_lock(&philo->first_fork->fork);
		write_message(philo, "has taken a fork", philo->id);
		pthread_mutex_lock(&philo->second_fork->fork);
		write_message(philo, "has taken a fork", philo->id);
		write_message(philo, "is eating", philo->id);
		ft_usleep(philo->data->time_to_eat);

		pthread_mutex_unlock(&philo->first_fork->fork);
		pthread_mutex_unlock(&philo->second_fork->fork);
		printf("Hi\n");
		// philo->data->dead = true;
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
			ft_usleep(10);
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
