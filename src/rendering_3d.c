/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_3d.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 23:26:56 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_textured_wall(t_game *game, t_ray *ray, int x)
{
	t_texture	*texture;
	int			tex_x;
	double		vars[2];
	int			color;
	int			y;

	texture = get_wall_texture(game, ray);
	tex_x = calculate_tex_x(ray, calculate_wall_x(game, ray), texture->width);
	vars[0] = 1.0 * texture->height / ray->line_height;
	vars[1] = (ray->draw_start - HEIGHT / 2 + ray->line_height / 2) * vars[0];
	y = ray->draw_start;
	while (y < ray->draw_end)
	{
		color = get_texture_color(texture, tex_x, (int)vars[1]
				& (texture->height - 1));
		if (ray->side == 1)
			color = (color >> 1) & 8355711;
		my_mlx_pixel_put(&game->img, x, y, color);
		vars[1] += vars[0];
		y++;
	}
}

void	draw_wall_stripe(t_game *game, t_ray *ray, int x)
{
	int	y;
	int	colors[2];

	colors[0] = (game->config.ceiling.r << 16)
		| (game->config.ceiling.g << 8) | game->config.ceiling.b;
	colors[1] = (game->config.floor.r << 16)
		| (game->config.floor.g << 8) | game->config.floor.b;
	y = 0;
	while (y < ray->draw_start)
	{
		my_mlx_pixel_put(&game->img, x, y, colors[0]);
		y++;
	}
	draw_textured_wall(game, ray, x);
	y = ray->draw_end;
	while (y < HEIGHT)
	{
		my_mlx_pixel_put(&game->img, x, y, colors[1]);
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
