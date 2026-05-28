/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stages.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:05:07 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/24 17:12:55 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
int	cool_down_check(t_coder *data, int proccess_t)
{
	struct timeval	current_time;
	struct timeval	start_time;

	gettimeofday(&current_time, NULL);
	gettimeofday(&start_time, NULL);
	while (current_time.tv_sec - start_time.tv_sec < proccess_t / 1000)
	{
		phtread_mutex_lock(&data->mutex);
		if (data->is_burnout == 1)
			return (-1);
		pthread_mutex_unlock($data->mutex);
		gettimeofday(&current_time, NULL);
	}
	return (0);
}

void	*stages(t_array *array)
{
	t_coder	*data;
	struct timeval	current_time;
	struct timeval	start_time;

	data = (t_coder *) array->data;
	if (cool_down_check(data, data->t_compile) != 0)
		return (-1)
	if (cool_down_check(data, data->t_debug) != 0)
		return (-1)
	if (cool_down_check(data, data->t_refactor) != 0)
		return (-1)
	phtread_mutex_lock(&data->mutex);
	data->is_done = 1;
	phtread_mutex_unlock(&data->mutex);
	return (NULL);
}
