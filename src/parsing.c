/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:27:25 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 18:36:11 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	parse_all_textures(t_config *config, const char *line)
{
	int	result;

	result = parse_texture_no(config, line);
	if (result != 0)
		return (result);
	result = parse_texture_so(config, line);
	if (result != 0)
		return (result);
	result = parse_texture_we(config, line);
	if (result != 0)
		return (result);
	result = parse_texture_ea(config, line);
	return (result);
}

static int	parse_all_colors(t_config *config, const char *line)
{
	int	result;

	result = parse_color_floor(config, line);
	if (result != 0)
		return (result);
	result = parse_color_ceiling(config, line);
	return (result);
}

static int	has_non_empty_content(const char *line)
{
	int		i;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (line[i] && line[i] != '\n')
		return (1);
	return (0);
}

int	parse_config_line(t_config *config, const char *line)
{
	int		result;

	result = parse_all_textures(config, line);
	if (result != 0)
		return (result);
	result = parse_all_colors(config, line);
	if (result != 0)
		return (result);
	if (has_non_empty_content(line))
		return (-1);
	return (0);
}

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
		if (result < 0)
		{
			free(line);
			return (-2);
		}
		free(line);
		line = get_next_line(fd);
	}
	return (0);
}

int	parse_config_file(t_config *config, const char *path)
{
	int		fd;
	int		result;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	result = read_config_lines(config, fd);
	close(fd);
	if (result == -1)
		ft_exit(1, "Failed to open map file");
	if (result == -2)
		ft_exit(1, "Invalid or duplicate identifier/s");
	if (result < 0)
		return (result);
	if ((config->config_flags & FLAG_ALL) != FLAG_ALL)
		ft_exit(1, "Missing identifier element/s");
	if (parse_map(config, path) < 0)
		ft_exit(1, "Invalid map format");
	return (0);
}
