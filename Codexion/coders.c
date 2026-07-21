/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:35:19 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/21 19:29:03 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	detect_b(t_coder *data)
{
	t_span	*s;

	s = (t_span *) data->s;
	while (1)
	{
		pthread_mutex_lock(&s->mut);
		if (s->is_over || s->is_over)
			return ((void) pthread_mutex_unlock(&s->mut));
		pthread_mutex_unlock(&s->mut);

		pthread_mutex_lock(&s->mut);
		gettimeofday(&data->b_interv_e, NULL);
		if ((data->b_interv_e.tv_sec * 1000000L + data->b_interv_e.tv_usec) - (data->b_interv_s.tv_sec * 1000000L + data->b_interv_s.tv_usec) > s->t_burnout * 1000)
		{
			pthread_mutex_unlock(&s->mut);
			break ;
		}
		pthread_mutex_unlock(&s->mut);
	}
	pthread_mutex_lock(&s->mut);
	s->is_failed = 1;
	printf("[%d ms] C%d BURNOUT\n", s->time, data->id);
	pthread_mutex_unlock(&s->mut);
}

void	coder(t_coder *data)
{
	t_span  		*s;
	unsigned int	n_comp;
	struct timespec wait;
    struct timeval  now;

	s = (t_span *) data->s;

    pthread_mutex_lock(&s->mut);
	n_comp = s->n_compiles;
	pthread_mutex_unlock(&s->mut);
	while(data->compiles != n_comp)
	{
		pthread_mutex_lock(&s->mut);
		while (!(data->conn[0] && data->conn[1]))
		{
			gettimeofday(&now, NULL);
            wait = convert(now, 300);
			if (pthread_cond_timedwait(&data->cond, &s->mut, &wait) == ETIMEDOUT)
            {
                if (s->is_failed || s->is_over)
                    return ((void) pthread_mutex_unlock(&s->mut));
                continue ;
            }
           break ;
		}
		printf("[%d ms] TAKE DONGLE C%d D%d\n", s->time, data->id, data->conn[1]->id);
		printf("[%d ms] TAKE DONGLE C%d D%d\n", s->time, data->id, data->conn[0]->id);
		pthread_mutex_unlock(&s->mut);
		if (stages(data) != 0)
			break ;
		pthread_mutex_lock(&s->mut);
		if (s->is_failed || s->is_over)
			return ((void) pthread_mutex_unlock(&s->mut));
		pthread_mutex_unlock(&s->mut);
		usleep(30);
	}
	pthread_mutex_lock(&s->mut);
	data->is_done = 1;
	pthread_mutex_unlock(&s->mut);

}

int init_arrays(t_span *s)
{
	t_array	  		*array;
	t_coder   		*data;

	array = NULL;
	while (la_len(la_start(array)) < s->n_coders){
		data = malloc(sizeof(t_coder));
		array = la_append(array, data);
		if (!array || !data)
			return (-1);
		data->id = la_len(la_start(array));
		pthread_cond_init(&data->cond, NULL);
		gettimeofday(&data->start, NULL);
		gettimeofday(&data->req_t, NULL);
		data->s = (void *) s;
		data->is_done = 0;
		data->is_active = 1;
		data->compiles = 0;
		data->conn[0] = NULL;
		data->conn[1] = NULL;
	}
	s->coders = la_start(array);
	return (0);
}
