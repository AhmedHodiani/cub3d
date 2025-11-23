/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:27:25 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 14:38:55 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	count_map_lines(int fd)
{
	char	*line;
	int		count;

	count = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (is_map_line(line))
			count++;
		free(line);
		line = get_next_line(fd);
	}
	return (count);
}

int	parse_map(t_config *config, const char *path)
{
	int		fd;
	int		line_count;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	line_count = count_map_lines(fd);
	close(fd);
	if (line_count == 0)
		return (-1);
	config->map.height = line_count;
	return (read_map_content(config, path));
}
