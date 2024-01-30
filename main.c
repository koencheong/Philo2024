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
	t_data	data;

	if (argc == 5 || argc == 6)
	{
		parse_input(&data, argv);
		data_init(&data);
		start_simulation(&data);
		// clean(&data);
		// printf("%ld\n", data.philo_nbr);
		// printf("%ld\n", data.time_to_die);
		// printf("%ld\n", data.time_to_eat);
		// printf("%ld\n", data.time_to_sleep);
		// printf("%ld\n", data.meals_nbr);
	}
	else
		error_exit("Wrong input.");
}