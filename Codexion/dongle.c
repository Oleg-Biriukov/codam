/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 10:16:37 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/24 16:35:46 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int init_dongle(t_span *s)
{
    t_array         *array;
    t_dongle        *data;
    pthread_mutex_t mutex;
    // pthread_cond_t  cond;

    array = NULL;
    pthread_mutex_init(&mutex, NULL);
    while(la_len(la_start(array)) != s->n_coders)
    {
        data = malloc(sizeof(t_dongle));
        array = la_append(array, data);
        if (!data || !array)
            return (-1);
        gettimeofday(&data->start, NULL);
        data->id = la_len(la_start(array));
        data->d_cooldown = s->d_cooldown;
        data->mutex = mutex;
        data->is_active = 1;
    }
    s->dongle = la_start(array);
    return (0);
}
