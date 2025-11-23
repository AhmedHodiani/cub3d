/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 23:26:56 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_player_north_south(t_game *game)
{
	if (game->config.map.player_dir == NORTH)
	{
		game->player.dir_x = 0;
		game->player.dir_y = -1;
		game->player.plane_x = 0.66;
		game->player.plane_y = 0;
	}
	else if (game->config.map.player_dir == SOUTH)
	{
		game->player.dir_x = 0;
		game->player.dir_y = 1;
		game->player.plane_x = -0.66;
		game->player.plane_y = 0;
	}
}

static void	init_player_west_east(t_game *game)
{
	if (game->config.map.player_dir == WEST)
	{
		game->player.dir_x = -1;
		game->player.dir_y = 0;
		game->player.plane_x = 0;
		game->player.plane_y = -0.66;
	}
	else if (game->config.map.player_dir == EAST)
	{
		game->player.dir_x = 1;
		game->player.dir_y = 0;
		game->player.plane_x = 0;
		game->player.plane_y = 0.66;
	}
}

void	init_player(t_game *game)
{
	game->player.x = game->config.map.player_pos[0] + 0.5;
	game->player.y = game->config.map.player_pos[1] + 0.5;
	init_player_north_south(game);
	init_player_west_east(game);
}

int	can_move_to(t_game *game, double new_x, double new_y)
{
	int	map_x;
	int	map_y;

	map_x = (int)new_x;
	map_y = (int)new_y;
	if (map_x < 0 || map_x >= game->config.map.width
		|| map_y < 0 || map_y >= game->config.map.height)
		return (0);
	if (game->config.map.grid[map_y][map_x] == WALL)
		return (0);
	return (1);
}
