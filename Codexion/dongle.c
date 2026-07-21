/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 10:16:37 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/21 12:34:38 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    dongle(t_dongle *data)
{
    t_span  *s;
    struct timespec wait;
    struct timeval  now;

    s = (t_span *) data->s;
    while (1)
    {
        pthread_mutex_lock(&s->mut);
		while (data->is_cooldown == 0)
        {
            gettimeofday(&now, NULL);
            wait = convert(now, 1000);
			if (pthread_cond_timedwait(&data->cond, &s->mut, &wait) == ETIMEDOUT)
            {
                if (s->is_failed || s->is_over)
                    return ((void) pthread_mutex_unlock(&s->mut));
                continue ;
            }
           break ;
        }   
		pthread_mutex_unlock(&s->mut);
        if (wait_check(s, s->d_cooldown) != 0)
            return ;
        pthread_mutex_lock(&s->mut);
        data->is_cooldown = 0;
        data->is_active = 1;
        pthread_mutex_unlock(&s->mut);
        usleep(30);
    }
    usleep(10);
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
        pthread_cond_init(&data->cond, NULL);
        data->id = la_len(la_start(array));
        data->s = (void *) s;
        data->is_cooldown = 0;
        data->is_active = 1;
    }
    s->dongle = la_start(array);
    return (0);
}
