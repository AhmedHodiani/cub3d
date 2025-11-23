/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:27:25 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 14:33:24 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_char_at(t_config *config, int x, int y)
{
	if (y < 0 || y >= config->map.height)
		return (' ');
	if (x < 0 || x >= (int)ft_strlen(config->map.grid[y]))
		return (' ');
	return (config->map.grid[y][x]);
}

static int	is_valid_neighbor(t_config *config, int x, int y)
{
	char	c;

	c = get_char_at(config, x, y);
	return (c == '1' || c == '0' || c == 'N'
		|| c == 'S' || c == 'E' || c == 'W');
}

static int	check_position(t_config *config, int x, int y)
{
	if (!is_valid_neighbor(config, x - 1, y))
		return (-1);
	if (!is_valid_neighbor(config, x + 1, y))
		return (-1);
	if (!is_valid_neighbor(config, x, y - 1))
		return (-1);
	if (!is_valid_neighbor(config, x, y + 1))
		return (-1);
	return (0);
}

int	check_internal_walls(t_config *config)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (i < config->map.height)
	{
		j = 0;
		while (config->map.grid[i][j])
		{
			c = config->map.grid[i][j];
			if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				if (check_position(config, j, i) < 0)
					return (-1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	check_sides(t_config *config, int row)
{
	int		len;
	int		i;

	len = ft_strlen(config->map.grid[row]);
	if (len == 0)
		return (0);
	i = 0;
	while (i < len && config->map.grid[row][i] == ' ')
		i++;
	if (i < len && config->map.grid[row][i] != '1')
		return (-1);
	i = len - 1;
	while (i >= 0 && config->map.grid[row][i] == ' ')
		i--;
	if (i >= 0 && config->map.grid[row][i] != '1')
		return (-1);
	return (0);
}
