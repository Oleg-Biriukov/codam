/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 10:16:37 by obirukov          #+#    #+#             */
/*   Updated: 2026/06/04 17:55:06 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int cool_down_d(t_dongle *data)
{
    struct timeval	current_time;
	struct timeval	start_time;
    t_span          *s;

    s = (t_span *) data->s;
    gettimeofday(&current_time, NULL);
    gettimeofday(&start_time, NULL);
	while (interval(start_time, current_time) < s->d_cooldown * 1000)
	{
		pthread_mutex_lock(&s->mutex_cod);
		if (s->is_failed == 1)
			return (pthread_mutex_unlock(&s->mutex_cod));
		pthread_mutex_unlock(&s->mutex_cod);
		gettimeofday(&current_time, NULL);
	}
    pthread_mutex_lock(&s->mutex_cod);
    data->is_cooldown = 0;
    return (pthread_mutex_unlock(&s->mutex_cod));
}

int    cool_down_dongle(t_span *s)
{
    t_dongle    *data;
    t_array     *a;

    a = s->workspace->next;
    while (1)
    {
        data = (t_dongle *)  a->data;
        pthread_mutex_lock(&s->mutex_cod);
        if (s->is_over || s->is_failed)
            return (pthread_mutex_unlock(&s->mutex_cod));
        pthread_mutex_unlock(&s->mutex_cod);
        pthread_mutex_lock(&s->mutex_cod);
        if (data->is_cooldown == 1)
        {
            pthread_mutex_unlock(&s->mutex_cod);
            if (pthread_create(&data->t, NULL, (void *) &cool_down_d, data) != 0)
                return (fail(s));
        }else
        {
            pthread_join(data->t, NULL);
        }
        pthread_mutex_unlock(&s->mutex_cod);
        a = a->next->next;
    }
}

int init_dongle(t_span *s)
{
    t_array         *array;
    t_dongle        *data;

    array = NULL;
    while(la_len(la_start(array)) != s->n_coders)
    {
        data = malloc(sizeof(t_dongle));
        array = la_append(array, data);
        if (!data || !array)
            return (-1);
        gettimeofday(&data->start, NULL);
        data->id = la_len(la_start(array));
        data->s = (void *) s;
        data->is_active = 1;
    }
    s->dongle = la_start(array);
    return (0);
}
