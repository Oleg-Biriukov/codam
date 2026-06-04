/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 12:33:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/06/04 17:36:57 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int fail(t_span *s)
{
    pthread_mutex_lock(&s->mutex_cod);
    s->is_failed = 1;
    return (pthread_mutex_unlock(&s->mutex_cod));
}

void	set_to_null(t_coder *data)
{
    if (data->conn[0])
        data->conn[0]->is_active = 1;
    if (data->conn[1])
        data->conn[1]->is_active = 1;
    data->conn[0] = NULL;
    data->conn[1] = NULL;
}


int start(t_span *s)
{
    int             compiles;
    pthread_t       t[3];
    t_array         *a;

    if (pthread_create(&t[0], NULL, (void *) &check_burnout, s))
        return (-1);
    if (pthread_create(&t[1], NULL, (void *) &cool_down_dongle, s))
        return (-1);
    if (pthread_create(&t[2], NULL, (void *) &scheduler, s))
        return (-1);
    a = s->coders;
    compiles = 0;
    while (compiles != s->n_compiles * s->n_coders)
    {
        pthread_mutex_lock(&s->mutex_cod);
        if (s->is_failed == 1)
            return (pthread_mutex_unlock(&s->mutex_cod), -1);
        pthread_mutex_unlock(&s->mutex_cod);
        compiles = 0;
        while (a)
        {
            compiles += ((t_coder *) a->data)->compiles;
            a = a->next;
        }
        a = s->coders;
    }
    pthread_mutex_lock(&s->mutex_cod);
    s->is_over = 1;
    pthread_mutex_unlock(&s->mutex_cod);
    if (pthread_join(t[0], NULL) != 0)
        return (-1);
    if (pthread_join(t[1], NULL) != 0)
        return (-1);
    if (pthread_join(t[2], NULL) != 0)
        return (-1);
    return (0);
}
