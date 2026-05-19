/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stages.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:05:07 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/19 17:04:12 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*stages(t_array *array)
{
	t_coder	*data;
	struct timeval	current_time;
	struct timeval	start_time;

	gettimeofday(&current_time, NULL);
	gettimeofday(&start_time, NULL);
	data = (t_coder *) array->data;
	printf("T%d is compiling...\n", data->id);
	while (current_time.tv_sec - start_time.tv_sec < data->t_compile / 1000)
	{
		if (data->is_burnout == 1)
			return (NULL);
		gettimeofday(&current_time, NULL);
	}
	gettimeofday(&start_time, NULL);
	printf("T%d is debugging...\n", data->id);
	while (current_time.tv_sec - start_time.tv_sec < data->t_debug / 1000)
	{
		if (data->is_burnout == 1)
			return (NULL);
		gettimeofday(&current_time, NULL);
	}
	gettimeofday(&start_time, NULL);
	printf("T%d is refactoring...\n", data->id);
	while (current_time.tv_sec - start_time.tv_sec < data->t_refactor / 1000)
	{
		if (data->is_burnout == 1)
			return (NULL);
		gettimeofday(&current_time, NULL);
	}
	data->is_done = 1;
	return (NULL);
}
