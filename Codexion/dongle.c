/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.codam.nl>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-05-18 10:16:37 by obirukov          #+#    #+#             */
/*   Updated: 2026-05-18 10:16:37 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int init_dongle(t_span *s)
{
    t_array         *array;
    t_dongle        *data;
    pthread_mutex_t mutex;

    array = NULL;
    pthread_mutex_init(&mutex, NULL);
    while(la_len(la_start(array)) != s->n_coders)
    {
        data = malloc(sizeof(t_dongle));
        array = la_append(array, data);
        if (!data || !array)
            return (-1);
        gettimeofday(&data->start, NULL);
        data->d_cooldown = s->d_cooldown;
        data->mutex = &mutex;
    }
    return (0);
}
