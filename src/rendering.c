/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 23:26:56 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	my_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	draw_minimap_tile(t_game *game, int x, int y, int color)
{
	int	i;
	int	j;
	int	screen_x;
	int	screen_y;

	i = 0;
	while (i < MINIMAP_SCALE)
	{
		j = 0;
		while (j < MINIMAP_SCALE)
		{
			screen_x = MINIMAP_OFFSET_X + x * MINIMAP_SCALE + i;
			screen_y = MINIMAP_OFFSET_Y + y * MINIMAP_SCALE + j;
			my_mlx_pixel_put(&game->img, screen_x, screen_y, color);
			j++;
		}
		i++;
	}
}

void	render_minimap(t_game *game)
{
	int		map_y;
	int		map_x;
	int		screen_y;
	int		screen_x;
	int		cam[2];

	get_camera_start(game, &cam[0], &cam[1]);
	screen_y = 0;
	while (screen_y < MINIMAP_VIEWPORT_TILES)
	{
		screen_x = 0;
		while (screen_x < MINIMAP_VIEWPORT_TILES)
		{
			map_x = cam[0] + screen_x;
			map_y = cam[1] + screen_y;
			draw_minimap_tile(game, screen_x, screen_y,
				get_tile_color(game, map_x, map_y));
			screen_x++;
		}
		screen_y++;
	}
}

void	draw_minimap_player(t_game *game)
{
	int	i;
	int	j;
	int	screen_x;
	int	screen_y;
	int	center[2];

	get_player_center(game, &center[0], &center[1]);
	i = -9;
	while (i <= 9)
	{
		j = -9;
		while (j <= 9)
		{
			screen_x = center[0] + i;
			screen_y = center[1] + j;
			if (i * i + j * j <= 27)
				my_mlx_pixel_put(&game->img, screen_x, screen_y,
					MINIMAP_COLOR_PLAYER);
			j++;
		}
		i++;
	}
}

void	render_frame(t_game *game)
{
	render_3d(game);
	render_minimap(game);
	draw_minimap_rays(game);
	draw_minimap_player(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
}
