/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcheong <kcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 21:51:09 by kcheong           #+#    #+#             */
/*   Updated: 2024/01/08 21:51:09 by kcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h> // printf
#include <stdlib.h> // malloc free
#include <unistd.h> // usleep
#include <stdbool.h>
#include <pthread.h> // mutex: init destroy lock unlock
					 // threads: create join detach
#include <sys/time.h> // gettimeofday
#include <limits.h> // INT_MAX

/*
	CODE FOR MUTEX / THREAD FUNCTIONS
*/
typedef enum	e_code
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}	t_ecode;

typedef pthread_mutex_t	t_mtx;

typedef struct s_fork
{
	t_mtx	fork;
	int		fork_id;
}	t_fork;

typedef struct s_table
{
	long	philo_nbr;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	meals_nbr;
	long	start_time;
	bool	end_simulation;
	t_fork	*forks;
	struct	s_philo	*philos;
}	t_table;

typedef struct s_philo
{
	int			id;
	long		meals_counter;
	bool		full;
	long		last_meal_time;
	t_fork		*first_fork;
	t_fork		*second_fork;
	pthread_t	thread_id;
	t_table		*table;
}	t_philo;


// utils
void	error_exit(const char *error);
long	ft_atol(const char *str);
void	parse_input(t_table *table, char **argv);

// init
void	data_init(t_table *table);

// safe functions
void	*safe_malloc(size_t size);

// philo
void	start_simulation(t_table *table);