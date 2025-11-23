/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:27:25 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 14:38:55 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_map_char(char c)
{
	return (c == '0' || c == '1' || c == 'N' || c == 'S'
		|| c == 'E' || c == 'W' || c == ' ');
}

int	is_map_line(const char *line)
{
	int	i;
	int	has_content;

	if (!line || !*line)
		return (0);
	i = 0;
	has_content = 0;
	while (line[i])
	{
		if (!is_map_char(line[i]) && line[i] != '\n')
			return (0);
		if (line[i] != ' ' && line[i] != '\n')
			has_content = 1;
		i++;
	}
	return (has_content);
}

int	get_line_length(const char *line)
{
	int	len;

	len = 0;
	while (line[len] && line[len] != '\n')
		len++;
	return (len);
}

int	store_map_line(t_config *config, char *line, int idx)
{
	int	len;

	len = get_line_length(line);
	if (len > config->map.width)
		config->map.width = len;
	config->map.grid[idx] = gc_strdup(line);
	if (!config->map.grid[idx])
		return (-1);
	if (config->map.grid[idx][len] == '\n')
		config->map.grid[idx][len] = '\0';
	return (0);
}
