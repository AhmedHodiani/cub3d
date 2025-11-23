/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_reading.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:27:25 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 23:26:56 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_non_empty_line(const char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	return (line[i] && line[i] != '\n');
}

static int	check_remaining_lines(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		if (is_non_empty_line(line))
			return (free(line), -1);
		free(line);
		line = get_next_line(fd);
	}
	return (0);
}

static int	process_map_lines(t_config *config, int fd)
{
	char	*line;
	int		idx;
	int		map_started;

	idx = 0;
	map_started = 0;
	line = get_next_line(fd);
	while (line && idx < config->map.height)
	{
		if (is_map_line(line))
		{
			map_started = 1;
			if (store_map_line(config, line, idx++) < 0)
				return (free(line), -1);
		}
		else if (map_started && is_non_empty_line(line))
			return (free(line), -1);
		free(line);
		line = get_next_line(fd);
	}
	if (line)
		free(line);
	return (check_remaining_lines(fd));
}

int	read_map_content(t_config *config, const char *path)
{
	int		fd;

	config->map.grid = gc_malloc((config->map.height + 1) * sizeof(char *));
	if (!config->map.grid)
		return (-1);
	ft_memset(config->map.grid, 0, (config->map.height + 1) * sizeof(char *));
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	if (process_map_lines(config, fd) < 0)
		return (close(fd), -1);
	close(fd);
	return (find_player(config));
}
