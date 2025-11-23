/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_3d.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 22:43:07 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

static t_texture	*get_wall_texture(t_game *game, t_ray *ray)
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

static void	draw_textured_wall(t_game *game, t_ray *ray, int x)
{
	t_texture	*texture;
	double		wall_x;
	int			tex_x;
	double		step;
	double		tex_pos;
	int			tex_y;
	int			color;
	int			y;

	texture = get_wall_texture(game, ray);
	if (ray->side == 0)
		wall_x = game->player.y + ray->perp_wall_dist * ray->ray_dir_y;
	else
		wall_x = game->player.x + ray->perp_wall_dist * ray->ray_dir_x;
	wall_x -= floor(wall_x);
	tex_x = (int)(wall_x * (double)texture->width);
	if (ray->side == 0 && ray->ray_dir_x > 0)
		tex_x = texture->width - tex_x - 1;
	if (ray->side == 1 && ray->ray_dir_y < 0)
		tex_x = texture->width - tex_x - 1;
	step = 1.0 * texture->height / ray->line_height;
	tex_pos = (ray->draw_start - HEIGHT / 2 + ray->line_height / 2) * step;
	y = ray->draw_start;
	while (y < ray->draw_end)
	{
		tex_y = (int)tex_pos & (texture->height - 1);
		tex_pos += step;
		color = get_texture_color(texture, tex_x, tex_y);
		if (ray->side == 1)
			color = (color >> 1) & 8355711;
		my_mlx_pixel_put(&game->img, x, y, color);
		y++;
	}
}

void	draw_wall_stripe(t_game *game, t_ray *ray, int x)
{
	int	y;
	int	ceiling_color;
	int	floor_color;

	ceiling_color = (game->config.ceiling.r << 16)
		| (game->config.ceiling.g << 8) | game->config.ceiling.b;
	floor_color = (game->config.floor.r << 16)
		| (game->config.floor.g << 8) | game->config.floor.b;
	y = 0;
	while (y < ray->draw_start)
	{
		my_mlx_pixel_put(&game->img, x, y, ceiling_color);
		y++;
	}
	draw_textured_wall(game, ray, x);
	y = ray->draw_end;
	while (y < HEIGHT)
	{
		my_mlx_pixel_put(&game->img, x, y, floor_color);
		y++;
	}
}

void	render_3d(t_game *game)
{
	t_ray	ray;
	int		x;

	x = 0;
	while (x < WIDTH)
	{
		init_ray(game, &ray, x);
		calculate_step_and_side_dist(game, &ray);
		perform_dda(game, &ray);
		calculate_wall_distance(game, &ray);
		draw_wall_stripe(game, &ray, x);
		x++;
	}
}
