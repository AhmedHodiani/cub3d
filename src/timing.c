/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 22:24:26 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <sys/time.h>
#include <unistd.h>

long	get_time_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}

void	update_delta_time(t_game *game)
{
	long	current_time;
	long	elapsed;

	current_time = get_time_us();
	elapsed = current_time - game->time.last_frame;
	game->time.delta_time = elapsed / 1000000.0;
	game->time.last_frame = current_time;
}

void	cap_framerate(t_game *game)
{
	long	current_time;
	long	elapsed;
	long	sleep_time;

	current_time = get_time_us();
	elapsed = current_time - game->time.last_frame;
	if (elapsed < FRAME_TIME_US)
	{
		sleep_time = FRAME_TIME_US - elapsed;
		usleep(sleep_time);
	}
}
