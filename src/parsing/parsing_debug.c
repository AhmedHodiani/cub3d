/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/28 15:26:57 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	print_map_char(char c)
{
	if (c == '1')
		ft_printf("██");
	else if (c == '0')
		ft_printf("  ");
	else if (c == ' ')
		ft_printf("··");
	else
		ft_printf("%c ", c);
}

void	print_map(char **grid, int height)
{
	int	i;
	int	j;

	ft_printf("Map grid:\n");
	i = 0;
	while (i < height)
	{
		j = 0;
		while (grid[i][j])
		{
			print_map_char(grid[i][j]);
			j++;
		}
		ft_printf("\n");
		i++;
	}
}

void	print_config(t_config *config)
{
	printf("North Texture: %s\n", config->textures.north);
	printf("South Texture: %s\n", config->textures.south);
	printf("West Texture: %s\n", config->textures.west);
	printf("East Texture: %s\n", config->textures.east);
	printf("Floor Color: R=%d, G=%d, B=%d\n",
		config->floor.r, config->floor.g, config->floor.b);
	printf("Ceiling Color: R=%d, G=%d, B=%d\n",
		config->ceiling.r, config->ceiling.g, config->ceiling.b);
	ft_printf("Map: %dx%d, Player at (%d, %d)\n",
		config->map.width, config->map.height,
		config->map.player_pos[0], config->map.player_pos[1]);
}
