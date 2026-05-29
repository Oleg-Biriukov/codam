/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 12:33:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/29 16:14:41 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
		gettimeofday(&c_data->start, NULL);
		c_data->conn[0] = NULL;
		c_data->conn[1] = NULL;
		c_data->is_burnout = 0;
		d_data->is_active = 1;
		c_a = c_a->next;
		d_a = d_a->next;
	}
}

int check_burnout(t_span *s)
{
    
}

int start(t_span *s)
{
    unsigned int    compiles;
    pthread_t       t[2];
    t_array         *a;
    t_coder         *data;
    int             res;

    compiles = 0;
    if (pthread_create(&t[1], NULL, (void *) &check_burnout, s))
        return (-1);
    while (compiles != s->n_compiles * s->n_coders)
    {
        a = s->coders;
        set_to_null(s);
        compiles = 0;
        if (pthread_create(&t[0], NULL, (void *) &scheduler, s) != 0)
            return (-1);
        while(a)
        {
            data = (t_coder *) a->data;
            if (pthread_create(&data->th_burnout, NULL, (void *) &proccess, a) != 0)
                return (-1);
            if (pthread_create(&data->th_burnout, NULL, (void *) &proccess, a) != 0)
                return (-1);
            a = a->next;
        }
        a = s->coders;
        while(a)
        {
            data = (t_coder *) a->data;
            if (pthread_join(data->th_burnout, (void *) &res) != 0)
                return (-1);
            compiles += data->compiles;
            a = a->next;
        }
        if (pthread_join(t, NULL) != 0)
            return (-1);
    }
    return (0);
}
