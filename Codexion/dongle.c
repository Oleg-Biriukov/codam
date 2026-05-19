/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 10:16:37 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/19 17:55:11 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    *cool_down(void *a)
{
    struct timeval  current_time;
    t_array         *array;
    t_dongle        *data;

    array = (t_array *) a;
    data = (t_dongle *) array->data;
	gettimeofday(&current_time, NULL);
    while (current_time.tv_sec - data->start.tv_sec != data->d_cooldown/1000)
		gettimeofday(&current_time, NULL);
    return NULL;
}

int init_dongle(t_span *s)
{
    t_array         *array;
    t_dongle        *data;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;

    array = NULL;
    pthread_mutex_init(&mutex, NULL);
    while(la_len(la_start(array)) != s->n_coders)
    {
        data = malloc(sizeof(t_dongle));
        array = la_append(array, data);
        if (!data || !array)
            return (-1);
        gettimeofday(&data->start, NULL);
        data->id = la_len(la_start(array)) - 1;
        data->d_cooldown = s->d_cooldown;
        data->mutex = mutex;
        data->is_active = 1;
        if (pthread_create(&data->t, NULL, &cool_down, array) != 0)
        {
            printf("Something went wrong with T%d", data->id);
            return (-1);
        }
    }
    s->dongle = la_start(array);
    return (0);
}
