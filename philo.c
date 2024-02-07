#include "philo.h"

void	start_routine(t_data *data)
{
	int	i;

	i = 0;
	if (data->philo_nbr == 1)
	{
		pthread_create(&data->philos[i].thread_id,
			NULL, &lonely_philo, &data->philos[i]);
		ft_usleep(50);
	}
	else
	{
		while (i < data->philo_nbr)
		{
			pthread_create(&data->philos[i].thread_id,
				NULL, &routine, &data->philos[i]);
			i++;
			ft_usleep(100);
		}
	}
	check_is_dead(data);
	i = -1;
	while (i++ < data->philo_nbr)
		pthread_join(data->philos[i].thread_id, NULL);
}

void	*lonely_philo(void *philoPassed)
{
	t_philo	*philo;

	philo = (t_philo *) philoPassed;
	pthread_mutex_lock(&philo->data->check_lock);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->data->check_lock);
	pthread_mutex_lock(&philo->first_fork->fork);
	write_message(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->first_fork->fork);
	while (anyone_died(philo) != true)
		ft_usleep(200);
	return (NULL);
}

void	write_message(t_philo *philo, char *message)
{
	if (anyone_died(philo) == true)
		return ;
	else
	{
		pthread_mutex_lock(&philo->write_lock);
		printf("%ld %d %s\n", get_time() - philo->start_time,
			philo->id, message);
		pthread_mutex_unlock(&philo->write_lock);
	}
}

void	wait_for_action(t_philo *philo, long time)
{
	long	start;

	start = get_time();
	while (anyone_died(philo) != true)
	{
		if (get_time() - start >= time)
			return ;
		ft_usleep(50);
	}
	return ;
}

// int	check_is_full(t_data *data, int i, int cont)
// {
// 	int	meals_eaten;
// 	int	required_meals;

// 	pthread_mutex_lock(&data->check_lock);
// 	meals_eaten = data->philos[i].meals_counter;
// 	required_meals = data->meals_nbr;
// 	pthread_mutex_unlock(&data->check_lock);
// 	if (meals_eaten == required_meals)
// 	{
// 		pthread_mutex_lock(&data->check_lock);
// 		data->philos[i].isDead = true;
// 		pthread_mutex_unlock(&data->check_lock);
// 		cont = 0;
// 	}
// 	return (cont);
// }

// void	check_is_dead2(t_data *data, int i, int cont)
// {
// 	long	time_to_die;
// 	long	last_meal;
// 	int		nbr_philo;

// 	pthread_mutex_lock(&data->check_lock);
// 	time_to_die = data->time_to_die;
// 	nbr_philo = data->philo_nbr;
// 	last_meal = data->philos[i].last_meal_time;
// 	pthread_mutex_unlock(&data->check_lock);
// 	while (cont && (i < nbr_philo))
// 	{
// 		if (get_time() - last_meal > time_to_die)
// 		{
// 			write_message(&data->philos[i], "died");
// 			pthread_mutex_lock(&data->check_lock);
// 			data->philos[i].isDead = true;
// 			pthread_mutex_unlock(&data->check_lock);
// 			cont = 0;
// 		}
// 		else
// 			cont = check_is_full(data, i, cont);
// 		i++;
// 	}
// }

void	check_is_dead(t_data *data)
{
	int		i;
	int		cont;
	long	time_to_die;
	long	last_meal;
	int		nbr_philo;
	int		meals_eaten;
	int		required_meals;

	cont = 1;
	while (cont == 1)
	{
		i = 0;
		pthread_mutex_lock(&data->check_lock);
		time_to_die = data->time_to_die;
		nbr_philo = data->philo_nbr;
		last_meal = data->philos[i].last_meal_time;
		meals_eaten = data->philos[i].meals_counter;
		required_meals = data->meals_nbr;
		pthread_mutex_unlock(&data->check_lock);
		while (cont && (i < nbr_philo))
		{
			if (get_time() - last_meal > time_to_die)
			{
				write_message(&data->philos[i], "died");
				pthread_mutex_lock(&data->check_lock);
				data->philos[i].isDead = true;
				pthread_mutex_unlock(&data->check_lock);
				cont = 0;
			}
			else if (meals_eaten == required_meals)
			{
				pthread_mutex_lock(&data->check_lock);
				data->philos[i].isDead = true;
				pthread_mutex_unlock(&data->check_lock);
				cont = 0;
			}
			i++;
		}
	}
}

int	anyone_died(t_philo *philo)
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

// void	philo_eat(t_philo *philo, long eat)
// {
// 	pthread_mutex_lock(&philo->first_fork->fork);
// 	write_message(philo, "has taken a fork");
// 	pthread_mutex_lock(&philo->second_fork->fork);
// 	write_message(philo, "has taken a fork");
// 	write_message(philo, "is eating");
// 	pthread_mutex_lock(&philo->data->check_lock);
// 	philo->last_meal_time = get_time();
// 	philo->meals_counter++;
// 	pthread_mutex_unlock(&philo->data->check_lock);
// 	wait_for_action(philo, eat);
// 	pthread_mutex_unlock(&philo->first_fork->fork);
// 	pthread_mutex_unlock(&philo->second_fork->fork);
// }

void	*routine(void *philoPassed)
{
	t_philo	*philo;
	long	eat;
	long	sleep;

	philo = (t_philo *) philoPassed;
	pthread_mutex_lock(&philo->data->check_lock);
	eat = philo->data->time_to_eat;
	sleep = philo->data->time_to_sleep;
	pthread_mutex_unlock(&philo->data->check_lock);
	while (anyone_died(philo) != true)
	{
		if (anyone_died(philo) != true)
		{
			pthread_mutex_lock(&philo->first_fork->fork);
			write_message(philo, "has taken a fork");
			pthread_mutex_lock(&philo->second_fork->fork);
			write_message(philo, "has taken a fork");
			write_message(philo, "is eating");
			pthread_mutex_lock(&philo->data->check_lock);
			philo->last_meal_time = get_time();
			philo->meals_counter++;
			pthread_mutex_unlock(&philo->data->check_lock);
			wait_for_action(philo, eat);
			pthread_mutex_unlock(&philo->first_fork->fork);
			pthread_mutex_unlock(&philo->second_fork->fork);
		}
		if (anyone_died(philo) != true)
		{
			write_message(philo, "is sleeping");
			wait_for_action(philo, sleep);
			write_message(philo, "is thinking");
		}
	}
	return (NULL);
}

