/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.codam.nl>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-05-27 12:33:15 by obirukov          #+#    #+#             */
/*   Updated: 2026-05-27 12:33:15 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int start(t_span *s)
{
    unsigned int    compiles;
    pthread_t       t;
    t_array         *a;
    t_coder         *data;
    int             res;

    compiles = 0;
    while (compiles != s->n_compiles * s->n_coders)
    {
        a = s->coders;
        set_to_null(s)
        compiles = 0;
        if (pthread_create(&t, NULL, &scheduler, s) != 0)
            return (-1);
        while(a)
        {
            data = (t_coder *) a->data;
            if (pthread_create(&data->th_burnout, NULL, &proccess, a) != 0)
                return (-1);
            a = a->next;
        }
        a = s->coders;
        while(a)
        {
            data = (t_coder *) a->data;
            if (pthread_join(data->th_burnout, &res) != 0)
                return (-1);
            compiles += data->compiles;
            a = a->next;
        }
    }
    return (0);
}
