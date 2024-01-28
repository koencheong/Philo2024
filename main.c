/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcheong <kcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 21:16:27 by kcheong           #+#    #+#             */
/*   Updated: 2024/01/09 21:16:27 by kcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc == 5 || argc == 6)
	{
		parse_input(&table, argv);
		data_init(&table);
		start_simulation(&table);
		// clean(&table);
		// printf("%ld\n", table.philo_nbr);
		// printf("%ld\n", table.time_to_die);
		// printf("%ld\n", table.time_to_eat);
		// printf("%ld\n", table.time_to_sleep);
		// printf("%ld\n", table.meals_nbr);
	}
	else
		error_exit("Wrong input.");
}