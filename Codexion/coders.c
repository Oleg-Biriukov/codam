/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:35:19 by obirukov          #+#    #+#             */
/*   Updated: 2026/06/04 16:27:12 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// start count down for each coders.
static int    cool_down_c(t_coder *data)
{
	t_span	*s;
    struct timeval	current_time;

	s = (t_span *) data->s;
    pthread_mutex_lock(&s->mutex_cod);
    gettimeofday(&data->start, NULL);
    pthread_mutex_unlock(&s->mutex_cod);

	gettimeofday(&current_time, NULL);
	while (interval(data->start, current_time) < s->t_burnout * 1000)
	{
		pthread_mutex_lock(&s->mutex_cod);
		if (data->is_done == 1)
			return (pthread_mutex_unlock(&s->mutex_cod));
		pthread_mutex_unlock(&s->mutex_cod);
		gettimeofday(&current_time, NULL);
	}
	pthread_mutex_lock(&s->mutex_cod);
    data->is_burnout = 1;
	return (pthread_mutex_unlock(&s->mutex_cod));
}

int    check_burnout(t_span *s)
{
    t_array         *a;
    t_coder         *data;

    a = s->coders;
	pthread_cond_wait(&s->cond_cod, &s->mutex_cond);
	pthread_mutex_unlock(&s->mutex_cond);
	while(a)
    {
        data = (t_coder *) a->data;
        if (pthread_create(&data->t, NULL, (void *) &proccess, a) != 0)
            return (-1);
        a = a->next;
    }
	a = s->coders;
    while (a)
    {
        data = (t_coder *) a->data;
        if (pthread_create(&data->th_burnout, NULL, (void *) &cool_down_c, data) != 0)
            return (fail(s));
        a = a->next;
    }
    a = s->workspace;
    while (a)
    {
        data = (t_coder *) a->data;
        if (data->is_burnout == 1)
            return (fail(s));
        if (data->is_done == 1)
        {	
			if (s->n_in_progress == 0)
			{
				pthread_mutex_lock(&s->mutex_cond);
				pthread_cond_broadcast(&s->cond_next);
				pthread_mutex_unlock(&s->mutex_cond);
				usleep(4);
			}
            pthread_join(data->th_burnout, NULL);
			set_to_null(data);
            if (pthread_create(&data->th_burnout, NULL, (void *) &cool_down_c, data) != 0)
                return (fail(s));
        }
        a = a->next->next;
    }
	return (0);
}

int proccess(t_array *a)
{
	t_coder	*data;
	t_span	*s;

	data = (t_coder *) a->data;
	s = (t_span *) data->s;
	while (1)
	{
		if (s->is_failed || s->is_over)
			return(-1);
		if (data->conn[0] && data->conn[1])
		{
			stages(a);
			pthread_mutex_lock(&s->mutex_cod);
			data->conn[0]->is_cooldown = 1;
			data->conn[1]->is_cooldown = 1;
			pthread_mutex_unlock(&s->mutex_cod);
		}
	}
	return (0);
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
		gettimeofday(&data->start, NULL);
		data->s = (void *) s;
		data->is_done = 0;
		data->is_burnout = 0;
		data->compiles = 0;
		data->conn[0] = NULL;
		data->conn[1] = NULL;
	}
	s->coders = la_start(array);
	return (0);
}
