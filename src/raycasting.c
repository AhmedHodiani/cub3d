/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 22:56:20 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

static void	draw_line(t_game *game, int x0, int y0, int x1, int y1)
{
	int		dx;
	int		dy;
	int		steps;
	float	x_inc;
	float	y_inc;
	float	x;
	float	y;
	int		i;
	int		max_x;
	int		max_y;

	dx = x1 - x0;
	dy = y1 - y0;
	steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	x_inc = dx / (float)steps;
	y_inc = dy / (float)steps;
	x = x0;
	y = y0;
	max_x = MINIMAP_OFFSET_X + MINIMAP_VIEWPORT_TILES * MINIMAP_SCALE;
	max_y = MINIMAP_OFFSET_Y + MINIMAP_VIEWPORT_TILES * MINIMAP_SCALE;
	i = 0;
	while (i <= steps)
	{
		if ((int)x >= MINIMAP_OFFSET_X && (int)x < max_x
			&& (int)y >= MINIMAP_OFFSET_Y && (int)y < max_y)
			my_mlx_pixel_put(&game->img, (int)x, (int)y, MINIMAP_COLOR_RAY);
		x += x_inc;
		y += y_inc;
		i++;
	}
}

static int	check_wall_hit(t_game *game, int map_x, int map_y)
{
	if (map_x < 0 || map_x >= game->config.map.width
		|| map_y < 0 || map_y >= game->config.map.height)
		return (1);
	if (game->config.map.grid[map_y][map_x] == WALL)
		return (1);
	return (0);
}

static void	cast_single_ray(t_game *game, double ray_dir_x, double ray_dir_y)
{
	double	ray_x;
	double	ray_y;
	double	step_x;
	double	step_y;
	int		map_x;
	int		map_y;
	int		screen_x;
	int		screen_y;
	int		i;
	int		cam_start_x;
	int		cam_start_y;

	ray_x = game->player.x;
	ray_y = game->player.y;
	step_x = ray_dir_x * 0.05;
	step_y = ray_dir_y * 0.05;
	cam_start_x = (int)game->player.x - MINIMAP_VIEWPORT_TILES / 2;
	cam_start_y = (int)game->player.y - MINIMAP_VIEWPORT_TILES / 2;
	i = 0;
	while (i < 200)
	{
		ray_x += step_x;
		ray_y += step_y;
		map_x = (int)ray_x;
		map_y = (int)ray_y;
		if (check_wall_hit(game, map_x, map_y))
			break ;
		i++;
	}
	screen_x = MINIMAP_OFFSET_X + (int)((ray_x - cam_start_x) * MINIMAP_SCALE);
	screen_y = MINIMAP_OFFSET_Y + (int)((ray_y - cam_start_y) * MINIMAP_SCALE);
	draw_line(game,
		MINIMAP_OFFSET_X + (MINIMAP_VIEWPORT_TILES / 2) * MINIMAP_SCALE
			+ (int)((game->player.x - (int)game->player.x) * MINIMAP_SCALE),
		MINIMAP_OFFSET_Y + (MINIMAP_VIEWPORT_TILES / 2) * MINIMAP_SCALE
			+ (int)((game->player.y - (int)game->player.y) * MINIMAP_SCALE),
		screen_x, screen_y);
}

void	draw_minimap_rays(t_game *game)
{
	int		i;
	double	ray_angle;
	double	ray_dir_x;
	double	ray_dir_y;
	double	fov;
	int		num_rays;

	fov = 60.0 * (3.14159265359 / 180.0);
	num_rays = 2000;
	i = 0;
	while (i < num_rays)
	{
		ray_angle = -fov / 2.0 + (fov * i / (num_rays - 1));
		ray_dir_x = game->player.dir_x * cos(ray_angle)
			- game->player.dir_y * sin(ray_angle);
		ray_dir_y = game->player.dir_x * sin(ray_angle)
			+ game->player.dir_y * cos(ray_angle);
		cast_single_ray(game, ray_dir_x, ray_dir_y);
		i++;
	}
}
