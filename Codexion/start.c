/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 12:33:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/17 14:16:43 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int fail(t_span *s)
{
    pthread_mutex_lock(&s->mut);
    s->is_failed = 1;
    return (pthread_mutex_unlock(&s->mut));
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
    unsigned int    counter;
    t_array         *a;
    t_coder         *c_data;
    t_dongle        *d_data;
    pthread_t       t;
    
    // creating new threads
    a = s->workspace;
    counter = 0;
    while (counter != s->n_coders * 2)
    {
        counter++;
        // if counter dividable to 2 then it is dongle
        if (counter % 2 == 0)
        {
            d_data = (t_dongle *) a->data;
            if (pthread_create(&d_data->t, NULL, (void *) dongle, d_data) != 0)
                return(fail(s));
        }
        else
        {
            c_data = (t_coder *) a->data;
            if (pthread_create(&c_data->t, NULL, (void *) coder, c_data) != 0)
                return(fail(s));
        }
        
        a = a->next;
    }
    if (pthread_create(&t, NULL, (void *) &scheduler, s) != 0)
        return (fail(s));
    
    // counter = 0;
    // a = s->workspace;
    while (la_len(s->coders) != 0)
    {
        if (s->is_failed)
            return (fail(s));
        // c_data = (t_coder *) a->data;
        // counter = 0;
        // if (c_data->compiles >= s->n_compiles)
        //     counter += s->n_compiles;
        // else
        //     counter += c_data->compiles;
        // a = a->next->next;
    }
    pthread_mutex_lock(&s->mut);
    s->is_over = 1;
    pthread_mutex_unlock(&s->mut);
    
    // awaiting for rest threads
    a = s->workspace;
    counter = 0;
    while (counter != s->n_coders * 2)
    {
        counter++;
        if (counter % 2 == 0)
        {
            d_data = (t_dongle *) a->data;
            pthread_join(d_data->t, NULL);
        }
        else
        {
            c_data = (t_coder *) a->data;
            pthread_join(c_data->t, NULL);
        }
        
        a = a->next;
    }
    pthread_join(t, NULL);
    return (0);
}
