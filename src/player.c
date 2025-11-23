/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 22:24:06 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

void	init_player(t_game *game)
{
	game->player.x = game->config.map.player_pos[0] + 0.5;
	game->player.y = game->config.map.player_pos[1] + 0.5;
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
	else if (game->config.map.player_dir == WEST)
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

static int	can_move_to(t_game *game, double new_x, double new_y)
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

void	move_forward(t_game *game, double move_speed)
{
	double	new_x;
	double	new_y;

	new_x = game->player.x + game->player.dir_x * move_speed;
	new_y = game->player.y + game->player.dir_y * move_speed;
	if (can_move_to(game, new_x, new_y))
	{
		game->player.x = new_x;
		game->player.y = new_y;
	}
}

void	move_backward(t_game *game, double move_speed)
{
	double	new_x;
	double	new_y;

	new_x = game->player.x - game->player.dir_x * move_speed;
	new_y = game->player.y - game->player.dir_y * move_speed;
	if (can_move_to(game, new_x, new_y))
	{
		game->player.x = new_x;
		game->player.y = new_y;
	}
}

void	strafe_left(t_game *game, double move_speed)
{
	double	new_x;
	double	new_y;

	new_x = game->player.x - game->player.plane_x * move_speed;
	new_y = game->player.y - game->player.plane_y * move_speed;
	if (can_move_to(game, new_x, new_y))
	{
		game->player.x = new_x;
		game->player.y = new_y;
	}
}

void	strafe_right(t_game *game, double move_speed)
{
	double	new_x;
	double	new_y;

	new_x = game->player.x + game->player.plane_x * move_speed;
	new_y = game->player.y + game->player.plane_y * move_speed;
	if (can_move_to(game, new_x, new_y))
	{
		game->player.x = new_x;
		game->player.y = new_y;
	}
}

void	rotate_left(t_game *game, double rot_speed)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player.dir_x;
	game->player.dir_x = game->player.dir_x * cos(-rot_speed)
		- game->player.dir_y * sin(-rot_speed);
	game->player.dir_y = old_dir_x * sin(-rot_speed)
		+ game->player.dir_y * cos(-rot_speed);
	old_plane_x = game->player.plane_x;
	game->player.plane_x = game->player.plane_x * cos(-rot_speed)
		- game->player.plane_y * sin(-rot_speed);
	game->player.plane_y = old_plane_x * sin(-rot_speed)
		+ game->player.plane_y * cos(-rot_speed);
}

void	rotate_right(t_game *game, double rot_speed)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player.dir_x;
	game->player.dir_x = game->player.dir_x * cos(rot_speed)
		- game->player.dir_y * sin(rot_speed);
	game->player.dir_y = old_dir_x * sin(rot_speed)
		+ game->player.dir_y * cos(rot_speed);
	old_plane_x = game->player.plane_x;
	game->player.plane_x = game->player.plane_x * cos(rot_speed)
		- game->player.plane_y * sin(rot_speed);
	game->player.plane_y = old_plane_x * sin(rot_speed)
		+ game->player.plane_y * cos(rot_speed);
}

void	process_movement(t_game *game)
{
	double	move_speed;
	double	rot_speed;

	move_speed = MOVE_SPEED * game->time.delta_time;
	rot_speed = ROT_SPEED * game->time.delta_time;
	if (game->keys.w)
		move_forward(game, move_speed);
	if (game->keys.s)
		move_backward(game, move_speed);
	if (game->keys.a)
		strafe_left(game, move_speed);
	if (game->keys.d)
		strafe_right(game, move_speed);
	if (game->keys.left)
		rotate_left(game, rot_speed);
	if (game->keys.right)
		rotate_right(game, rot_speed);
}
