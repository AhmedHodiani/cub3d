/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 23:26:56 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_wall_hit(t_game *game, int map_x, int map_y)
{
	if (map_x < 0 || map_x >= game->config.map.width
		|| map_y < 0 || map_y >= game->config.map.height)
		return (1);
	if (game->config.map.grid[map_y][map_x] == WALL)
		return (1);
	return (0);
}

int	get_max_steps(int dx, int dy)
{
	if (abs(dx) > abs(dy))
		return (abs(dx));
	return (abs(dy));
}

void	init_line_vars(int *coords, float *vars)
{
	vars[0] = (coords[2] - coords[0]) / vars[2];
	vars[1] = (coords[3] - coords[1]) / vars[2];
	vars[3] = coords[0];
	vars[4] = coords[1];
}
