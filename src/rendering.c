/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 22:56:20 by ataher           ###   ########.fr       */
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
	int		color;
	int		cam_start_x;
	int		cam_start_y;

	cam_start_x = (int)game->player.x - MINIMAP_VIEWPORT_TILES / 2;
	cam_start_y = (int)game->player.y - MINIMAP_VIEWPORT_TILES / 2;
	screen_y = 0;
	while (screen_y < MINIMAP_VIEWPORT_TILES)
	{
		screen_x = 0;
		while (screen_x < MINIMAP_VIEWPORT_TILES)
		{
			map_x = cam_start_x + screen_x;
			map_y = cam_start_y + screen_y;
			if (map_y >= 0 && map_y < game->config.map.height
				&& map_x >= 0 && map_x < game->config.map.width)
			{
				if (game->config.map.grid[map_y][map_x] == WALL)
					color = MINIMAP_COLOR_WALL;
				else if (game->config.map.grid[map_y][map_x] == WALKABLE)
					color = MINIMAP_COLOR_FLOOR;
				else
					color = 0x000000;
			}
			else
				color = 0x000000;
			draw_minimap_tile(game, screen_x, screen_y, color);
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
	int	center_x;
	int	center_y;
	double	player_offset_x;
	double	player_offset_y;

	player_offset_x = game->player.x - (int)game->player.x;
	player_offset_y = game->player.y - (int)game->player.y;
	center_x = MINIMAP_OFFSET_X + (MINIMAP_VIEWPORT_TILES / 2) * MINIMAP_SCALE
		+ (int)(player_offset_x * MINIMAP_SCALE);
	center_y = MINIMAP_OFFSET_Y + (MINIMAP_VIEWPORT_TILES / 2) * MINIMAP_SCALE
		+ (int)(player_offset_y * MINIMAP_SCALE);
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
	render_3d(game);
	render_minimap(game);
	draw_minimap_rays(game);
	draw_minimap_player(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
}
