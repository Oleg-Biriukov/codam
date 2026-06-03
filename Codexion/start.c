/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 12:33:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/06/03 16:23:05 by obirukov         ###   ########.fr       */
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
	t_dongle	*d_data;
	
    d_data = (t_dongle *) data->conn[0];
    d_data->is_active = 1;
    d_data = (t_dongle *) data->conn[1];
    d_data->is_active = 1;
	data->conn[0] = NULL;
	data->conn[1] = NULL;
}


int start(t_span *s)
{
    unsigned int    compiles;
    pthread_t       t[2];
    t_array         *a;

    if (pthread_create(&t[1], NULL, (void *) &check_burnout, s))
        return (-1);
    if (pthread_create(&t[0], NULL, (void *) &cool_down_dongle, s))
        return (-1);
    compiles = 0;
    while (compiles != s->n_compiles * s->n_coders)
    {
        pthread_mutex_lock(&s->mutex_g);
        if (s->is_failed == 1)
            return (pthread_mutex_unlock(&s->mutex_g), -1);
        pthread_mutex_unlock(&s->mutex_g);
        a = s->coders;
        
        
        a = s->coders;
    }
    pthread_mutex_lock(&s->mutex_g);
    s->is_over = 1;
    pthread_mutex_unlock(&s->mutex_g);
    if (pthread_join(t[1], NULL) != 0)
        return (-1);
    if (pthread_join(t[0], NULL) != 0)
        return (-1);
    return (0);
}
