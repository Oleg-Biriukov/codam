/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 10:16:37 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/17 16:42:33 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    dongle(t_dongle *data)
{
    t_span  *s;

    s = (t_span *) data->s;
    while (1)
    {
        pthread_mutex_lock(&s->mut);
		while (data->is_cooldown == 0)
			pthread_cond_wait(&data->cond, &s->mut);
        printf("[%d] Waiting for cooldown for d%d\n", s->time, data->id);
		pthread_mutex_unlock(&s->mut);
        if (wait_check(s, s->d_cooldown) != 0)
            return ;
        printf("[%d] Cooldown finished for d%d\n", s->time, data->id);
        pthread_mutex_lock(&s->mut);
        data->is_cooldown = 0;
        data->is_active = 1;
        pthread_mutex_unlock(&s->mut);
    }
    usleep(10);
    // la_len(s->coders);
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
