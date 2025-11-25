/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/24 15:51:35 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

static void	draw_line(t_game *game, int *coords)
{
	int		i;
	int		max[2];
	float	vars[5];

	vars[2] = get_max_steps(coords[2] - coords[0], coords[3] - coords[1]);
	init_line_vars(coords, vars);
	max[0] = MINIMAP_OFFSET_X + MINIMAP_VIEWPORT_TILES * MINIMAP_SCALE;
	max[1] = MINIMAP_OFFSET_Y + MINIMAP_VIEWPORT_TILES * MINIMAP_SCALE;
	i = 0;
	while (i <= (int)vars[2])
	{
		if ((int)vars[3] >= MINIMAP_OFFSET_X && (int)vars[3] < max[0]
			&& (int)vars[4] >= MINIMAP_OFFSET_Y && (int)vars[4] < max[1])
			my_mlx_pixel_put(&game->img, (int)vars[3], (int)vars[4],
				MINIMAP_COLOR_RAY);
		vars[3] += vars[0];
		vars[4] += vars[1];
		i++;
	}
}

static void	trace_ray(t_game *game, double *ray, double *step)
{
	int	i;
	int	map_x;
	int	map_y;

	i = 0;
	while (i < 200)
	{
		ray[0] += step[0];
		ray[1] += step[1];
		map_x = (int)ray[0];
		map_y = (int)ray[1];
		if (check_wall_hit(game, map_x, map_y))
			break ;
		i++;
	}
}

static void	cast_single_ray(t_game *game, double ray_dir_x, double ray_dir_y)
{
	double	ray[2];
	double	step[2];
	int		cam[2];
	int		coords[4];

	ray[0] = game->player.x;
	ray[1] = game->player.y;
	step[0] = ray_dir_x * 0.05;
	step[1] = ray_dir_y * 0.05;
	get_camera_start(game, &cam[0], &cam[1]);
	trace_ray(game, ray, step);
	coords[0] = MINIMAP_OFFSET_X + (MINIMAP_VIEWPORT_TILES / 2) * MINIMAP_SCALE
		+ (int)((game->player.x - (int)game->player.x) * MINIMAP_SCALE);
	coords[1] = MINIMAP_OFFSET_Y + (MINIMAP_VIEWPORT_TILES / 2) * MINIMAP_SCALE
		+ (int)((game->player.y - (int)game->player.y) * MINIMAP_SCALE);
	coords[2] = MINIMAP_OFFSET_X + (int)((ray[0] - cam[0]) * MINIMAP_SCALE);
	coords[3] = MINIMAP_OFFSET_Y + (int)((ray[1] - cam[1]) * MINIMAP_SCALE);
	draw_line(game, coords);
}

void	draw_minimap_rays(t_game *game)
{
	int		i;
	double	ray_angle;
	double	ray_dir[2];
	double	fov;
	int		num_rays;

	fov = FOV_RAD;
	num_rays = 1000;
	i = 0;
	while (i < num_rays)
	{
		ray_angle = -fov / 2.0 + (fov * i / (num_rays - 1));
		ray_dir[0] = game->player.dir_x * cos(ray_angle)
			- game->player.dir_y * sin(ray_angle);
		ray_dir[1] = game->player.dir_x * sin(ray_angle)
			+ game->player.dir_y * cos(ray_angle);
		cast_single_ray(game, ray_dir[0], ray_dir[1]);
		i++;
	}
}
