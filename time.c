/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brendos- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 11:35:42 by brendos-          #+#    #+#             */
/*   Updated: 2026/01/25 11:35:44 by brendos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((long long)time.tv_sec * 1000
		+ (long long)time.tv_usec / 1000);
}

void smart_sleep(t_table *table, long time_to_sleep)
{
    long start = get_time_ms();
    while (get_time_ms() - start < time_to_sleep)
    {
        pthread_mutex_lock(&table->lock_mtx);
        if (table->stop_simulation) // Agora a leitura está protegida!
        {
            pthread_mutex_unlock(&table->lock_mtx);
            return ;
        }
        pthread_mutex_unlock(&table->lock_mtx);
        usleep(500);
    }
}
⁩
