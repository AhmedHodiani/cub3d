/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:27:25 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 23:26:56 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	read_config_lines(t_config *config, int fd)
{
	char	*line;
	int		result;

	line = get_next_line(fd);
	while (line != NULL)
	{
		if (is_map_line(line))
		{
			free(line);
			break ;
		}
		result = parse_config_line(config, line);
		free(line);
		if (result < 0)
			return (-2);
		line = get_next_line(fd);
	}
	return (0);
}

int	parse_config_file(t_config *config, char *path)
{
	int		fd;
	int		result;
	size_t	len;

	ft_memset(config, 0, sizeof(t_config));
	len = ft_strlen(path);
	if (len < 4 || ft_strcmp(path + len - 4, ".cub") != 0)
		ft_exit(2, "Map file must have .cub extension");
	fd = open(path, O_RDONLY);
	if (fd < 0)
		ft_exit(3, "Failed to open map file");
	result = read_config_lines(config, fd);
	close(fd);
	if (result == -2)
		ft_exit(4, "Invalid or duplicate identifier/s");
	if ((config->config_flags & FLAG_ALL) != FLAG_ALL)
		ft_exit(5, "Missing identifier element/s");
	if (parse_map(config, path) < 0)
		ft_exit(6, "Invalid map format");
	print_config(config);
	print_map(config->map.grid, config->map.height);
	if (validate_map_walls(config) < 0)
		ft_exit(1, "Map not properly closed by walls");
	return (0);
}
