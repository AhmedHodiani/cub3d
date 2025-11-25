/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 23:26:56 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_tile_color(t_game *game, int map_x, int map_y)
{
	int	line_len;

	if (map_y >= 0 && map_y < game->config.map.height && map_x >= 0)
	{
		line_len = ft_strlen(game->config.map.grid[map_y]);
		if (map_x < line_len)
		{
			if (game->config.map.grid[map_y][map_x] == WALL)
				return (MINIMAP_COLOR_WALL);
			else if (game->config.map.grid[map_y][map_x] == WALKABLE)
				return (MINIMAP_COLOR_FLOOR);
		}
	}
	return (0x000000);
}

void	get_camera_start(t_game *game, int *cam_start_x, int *cam_start_y)
{
	*cam_start_x = (int)game->player.x - MINIMAP_VIEWPORT_TILES / 2;
	*cam_start_y = (int)game->player.y - MINIMAP_VIEWPORT_TILES / 2;
}

void	get_player_center(t_game *game, int *center_x, int *center_y)
{
	double	player_offset_x;
	double	player_offset_y;

	player_offset_x = game->player.x - (int)game->player.x;
	player_offset_y = game->player.y - (int)game->player.y;
	*center_x = MINIMAP_OFFSET_X + (MINIMAP_VIEWPORT_TILES / 2) * MINIMAP_SCALE
		+ (int)(player_offset_x * MINIMAP_SCALE);
	*center_y = MINIMAP_OFFSET_Y + (MINIMAP_VIEWPORT_TILES / 2) * MINIMAP_SCALE
		+ (int)(player_offset_y * MINIMAP_SCALE);
}
