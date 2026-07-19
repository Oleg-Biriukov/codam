/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 12:33:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/19 14:15:25 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int fail(t_span *s)
{
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
    unsigned int    total_c;
    unsigned int    n_coders;
    t_array         *a;
    t_array         *workspace;
    t_coder         *c_data;
    t_dongle        *d_data;
    pthread_t       t;
    
    // creating new threads
    pthread_mutex_lock(&s->mut);
    workspace = s->workspace;
    pthread_mutex_unlock(&s->mut);
    counter = 0;
    a = workspace;
    while (counter != s->n_coders * 2)
    {
        counter++;
        // if counter dividable to 2 then it is dongle
        if (counter % 2 == 0)
        {
            d_data = (t_dongle *) a->data;
            pthread_mutex_lock(&s->mut);
            if (pthread_create(&d_data->t, NULL, (void *) dongle, d_data) != 0)
                return(fail(s));
            pthread_mutex_unlock(&s->mut);
        }
        else
        {
            c_data = (t_coder *) a->data;
            pthread_mutex_lock(&s->mut);
            if (pthread_create(&c_data->t, NULL, (void *) coder, c_data) != 0)
                return(fail(s));
            pthread_mutex_unlock(&s->mut);
        }
        
        a = a->next;
    }
    pthread_mutex_lock(&s->mut);
    if (pthread_create(&t, NULL, (void *) &scheduler, s) != 0)
        return (fail(s));
    total_c = s->n_coders * s->n_compiles;
    n_coders = s->n_coders;
    pthread_mutex_unlock(&s->mut);
    
    counter = 0;
    a = workspace;
    while (1)
    {
        if (counter == total_c)
            break ;
        if (a == s->workspace)
            counter = 0;
        pthread_mutex_lock(&s->mut);
        if (s->is_failed)
            return (fail(s));
        c_data = (t_coder *) a->data;
        if (c_data->compiles >= s->n_compiles)
            counter += s->n_compiles;
        else
            counter += c_data->compiles;
        pthread_mutex_unlock(&s->mut);
        a = a->next->next;
        usleep(30);
    }
    pthread_mutex_lock(&s->mut);
    s->is_over = 1;
    pthread_mutex_unlock(&s->mut);
    
    // awaiting for rest threads
    a = workspace;
    counter = 0;
    while (counter != n_coders * 2)
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
