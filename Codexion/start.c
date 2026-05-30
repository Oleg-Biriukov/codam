/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 12:33:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/30 17:07:44 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void fail(t_span *s)
{
    pthread_mutex_lock(&s->mutex_g);
    s->is_failed = 1;
    pthread_mutex_unlock(&s->mutex_g);
}

// start count down for each coders.
void    cool_down_b(t_array *a)
{
    t_coder	*data;
	t_span	*s;
    struct timeval	current_time;
	struct timeval	start_time;

	data = (t_coder *) a->data;
	s = (t_span *) data->s;
    pthread_mutex_lock(&s->mutex_cod);
    gettimeofday(&data->start, NULL);
    pthread_mutex_unlock(&s->mutex_cod);

	gettimeofday(&current_time, NULL);
	while (current_time.tv_sec - data->start.tv_sec < s->t_burnout / 1000)
	{
		pthread_mutex_lock(&s->mutex_cod);
		if (data->is_burnout == 1)
			return ;
		pthread_mutex_unlock(&s->mutex_cod);
		gettimeofday(&current_time, NULL);
	}
    return ;
}

void	set_to_null(t_span *s)
{
	t_array		*c_a;
	t_array		*d_a;
	t_coder		*c_data;
	t_dongle	*d_data;

	c_a = s->coders;
	d_a = s->dongle;
	while (c_a)
	{

		c_data = (t_coder *) c_a->data;
		d_data = (t_dongle *) d_a->data;
		c_data->conn[0] = NULL;
		c_data->conn[1] = NULL;
		c_data->is_burnout = 0;
		d_data->is_active = 1;
		c_a = c_a->next;
		d_a = d_a->next;
	}
}

void    check_burnout(t_span *s)
{
    unsigned int    compiles;
    t_array         *a;
    t_coder         *data;
    int             res;

    compiles = 0;
    a = s->coders;
    while (a)
    {
        data = (t_coder *) a->data;
        if (pthread_create(&data->th_burnout, NULL, (void *) &cool_down_b, a) != 0)
            return (fail(s));
        a = a->next;
    }
    a = s->workspace;
    while (a)
    {
        data = (t_coder *) ((t_array *) a->data)->data;
        if (data->is_burnout)
            return (fail(s));
        if (data->is_done)
        {
            pthread_join(data->th_burnout, NULL);
            if (pthread_create(&data->th_burnout, NULL, (void *) &cool_down_b, a) != 0)
                return (fail(s));
        }
        a = a->next->next;   
    }
}

int start(t_span *s)
{
    unsigned int    compiles;
    pthread_t       t[2];
    t_array         *a;
    t_coder         *data;
    int             res;

    if (pthread_create(&t[1], NULL, (void *) &check_burnout, s))
        return (-1);
    while (compiles != s->n_compiles * s->n_coders)
    {
        pthread_mutex_lock(&s->mutex_g);
        if (s->is_failed == 1)
            return (pthread_mutex_unlock(&s->mutex_g), -1);
        pthread_mutex_unlock(&s->mutex_g);
        a = s->coders;
        set_to_null(s);
        if (scheduler(s) != 0)
            return (-1);
        while(a)
        {
            data = (t_coder *) a->data;
            if (pthread_create(&data->t, NULL, (void *) &proccess, a) != 0)
                return (-1);
            a = a->next;
        }
        a = s->coders;
    }
    if (pthread_join(t[1], NULL))
        return (-1);
    return (0);
}
