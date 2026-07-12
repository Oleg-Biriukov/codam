/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 10:16:37 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/12 15:41:08 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    dongle(t_dongle *data)
{
    t_span  *s;

    s = (t_span *) data->s;
    // printf("[%d ms] Dongle id %d\n", s->time, data->id);
    la_len(s->coders);
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
