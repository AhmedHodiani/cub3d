/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:27:25 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 14:38:55 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_direction	get_direction(char c)
{
	if (c == 'N')
		return (NORTH);
	if (c == 'S')
		return (SOUTH);
	if (c == 'W')
		return (WEST);
	return (EAST);
}

static int	check_player_char(t_config *config, int i, int j, int *found)
{
	if (config->map.grid[i][j] == 'N'
		|| config->map.grid[i][j] == 'S'
		|| config->map.grid[i][j] == 'E'
		|| config->map.grid[i][j] == 'W')
	{
		if ((*found)++)
			return (-1);
		config->map.player_pos[0] = j;
		config->map.player_pos[1] = i;
		config->map.player_dir = get_direction(config->map.grid[i][j]);
	}
	return (0);
}

int	find_player(t_config *config)
{
	int		i;
	int		j;
	int		found;

	found = 0;
	i = -1;
	while (++i < config->map.height)
	{
		j = -1;
		while (config->map.grid[i][++j])
		{
			if (check_player_char(config, i, j, &found) < 0)
				return (-1);
		}
	}
	if (found == 1)
		return (0);
	return (-1);
}

static int	check_wall_line(const char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != '1' && line[i] != ' ')
			return (-1);
		i++;
	}
	return (0);
}

int	validate_map_walls(t_config *config)
{
	int	i;

	if (check_wall_line(config->map.grid[0]) < 0)
		return (-1);
	if (check_wall_line(config->map.grid[config->map.height - 1]) < 0)
		return (-1);
	i = 0;
	while (i < config->map.height)
	{
		if (check_sides(config, i) < 0)
			return (-1);
		i++;
	}
	return (check_internal_walls(config));
}
