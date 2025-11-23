/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 22:15:48 by ataher           ###   ########.fr       */
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
	int		y;
	int		x;
	int		color;

	y = 0;
	while (y < game->config.map.height)
	{
		x = 0;
		while (x < game->config.map.width)
		{
			if (game->config.map.grid[y][x] == WALL)
				color = MINIMAP_COLOR_WALL;
			else if (game->config.map.grid[y][x] == WALKABLE)
				color = MINIMAP_COLOR_FLOOR;
			draw_minimap_tile(game, x, y, color);
			x++;
		}
		y++;
	}
}

void	draw_minimap_player(t_game *game)
{
	int	i;
	int	j;
	int	screen_x;
	int	screen_y;
	int	center_x;
	int	center_y;

	center_x = MINIMAP_OFFSET_X + (int)(game->player.x * MINIMAP_SCALE);
	center_y = MINIMAP_OFFSET_Y + (int)(game->player.y * MINIMAP_SCALE);
	i = -9;
	while (i <= 9)
	{
		j = -9;
		while (j <= 9)
		{
			screen_x = center_x + i;
			screen_y = center_y + j;
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
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			my_mlx_pixel_put(&game->img, x, y, 0x000000);
			x++;
		}
		y++;
	}
	render_minimap(game);
	draw_minimap_rays(game);
	draw_minimap_player(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
}
