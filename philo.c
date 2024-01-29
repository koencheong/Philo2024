#include "philo.h"

void	*dinner_simulation(void *data)
{
	t_philo *philo;

	philo = (t_philo *) data;
	printf("Hello from Philo ID [%d]\n", philo->id);

	return (NULL);
}

void	start_simulation(t_table *table)
{
	int i;

	i = 0;
	if (table->meals_nbr == 0)
		return ;
	else if (table->meals_nbr == 1)
		; //TODO
	else
	{
		while (i < table->philo_nbr)
		{
			// printf("i is %d\n", i);
			pthread_create(&table->philos[i].thread_id, NULL, dinner_simulation, &table->philos[i]);
			ft_usleep(1);
			i++;
		}
		i = 0;
		while ( i < table->philo_nbr)
		{
			pthread_join(table->philos[i].thread_id, NULL);
			i++;
		}
	}
}