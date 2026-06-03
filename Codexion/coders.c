/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:35:19 by obirukov          #+#    #+#             */
/*   Updated: 2026/06/03 14:36:58 by obirukov         ###   ########.fr       */
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
	while (current_time.tv_usec - data->start.tv_usec < s->t_burnout * 1000)
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
    unsigned int    compiles;
    t_array         *a;
    t_coder         *data;

    compiles = 0;
    a = s->coders;

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
	if (data->conn[0] && data->conn[1])
		stages(a);
	return (0);
}


int init_arrays(t_span *s)
{
	pthread_mutex_t mutex;
	t_array	  		*array;
	t_coder   		*data;

	array = NULL;
	pthread_mutex_init(&mutex, NULL);
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
	
		// if (pthread_create(&data->th_burnout, NULL, (void *) &proccess, array) != 0 ||
		// 	pthread_create(&data->th_stages, NULL, (void *) &stages, array) != 0)
		// 	return (-2);
	}
	s->coders = la_start(array);
	return (0);
}
