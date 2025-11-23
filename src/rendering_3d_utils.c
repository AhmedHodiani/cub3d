/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_3d_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 23:26:56 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

t_texture	*get_wall_texture(t_game *game, t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->ray_dir_x > 0)
			return (&game->textures[EAST]);
		else
			return (&game->textures[WEST]);
	}
	else
	{
		if (ray->ray_dir_y > 0)
			return (&game->textures[SOUTH]);
		else
			return (&game->textures[NORTH]);
	}
}

int	calculate_tex_x(t_ray *ray, double wall_x, int tex_width)
{
	int	tex_x;

	tex_x = (int)(wall_x * (double)tex_width);
	if (ray->side == 0 && ray->ray_dir_x > 0)
		tex_x = tex_width - tex_x - 1;
	if (ray->side == 1 && ray->ray_dir_y < 0)
		tex_x = tex_width - tex_x - 1;
	return (tex_x);
}

double	calculate_wall_x(t_game *game, t_ray *ray)
{
	double	wall_x;

	if (ray->side == 0)
		wall_x = game->player.y + ray->perp_wall_dist * ray->ray_dir_y;
	else
		wall_x = game->player.x + ray->perp_wall_dist * ray->ray_dir_x;
	wall_x -= floor(wall_x);
	return (wall_x);
}
