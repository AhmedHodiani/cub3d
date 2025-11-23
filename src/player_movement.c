/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 23:26:56 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
