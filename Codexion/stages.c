/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stages.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:05:07 by obirukov          #+#    #+#             */
/*   Updated: 2026/06/05 16:59:12 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
int	cool_down_check(t_coder *data, int proccess_t)
{
	struct timeval	current_time;
	struct timeval	start_time;
	t_span			*s;

	s = (t_span *) data->s;
	gettimeofday(&current_time, NULL);
	gettimeofday(&start_time, NULL);
	while (interval(start_time, current_time) < proccess_t * 1000)
	{
		pthread_mutex_lock(&s->mutex_cod);
		if (data->is_burnout == 1)
			return (pthread_mutex_unlock(&s->mutex_cod), -1);
		pthread_mutex_unlock(&s->mutex_cod);
		gettimeofday(&current_time, NULL);
	}
	return (0);
}

int		stages(t_array *array)
{
	t_coder			*data;
	t_span			*s;

	data = (t_coder *) array->data;
	s = (t_span *) data->s;
	printf("[%d ms] START_COMPILE\tC%d\n", s->time , data->id);
	pthread_mutex_lock(&s->mutex_cod);
	data->compiles += 1;
	s->n_in_progress--;
	pthread_mutex_unlock(&s->mutex_cod);
	if (cool_down_check(data, s->t_compile) != 0)
		return (-1);
	pthread_mutex_lock(&s->mutex_cod);
	data->compiles += 1;
	pthread_mutex_unlock(&s->mutex_cod);
	printf("[%d ms] START_DEBUG\tC%d\n", s->time , data->id);
	if (cool_down_check(data, s->t_debug) != 0)
		return (-1);
	printf("[%d ms] START_REFACTOR\tC%d\n", s->time , data->id);
	if (cool_down_check(data, s->t_refactor) != 0)
		return (-1);
	// pthread_mutex_lock(&s->mutex_cod);
	// data->is_done = 1;
	// pthread_mutex_unlock(&s->mutex_cod);
	return (0);
}
