/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:27:25 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 14:19:36 by ataher           ###   ########.fr       */
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

int	parse_config_line(t_config *config, const char *line)
{
	int	result;

	result = parse_all_textures(config, line);
	if (result != 0)
		return (result);
	result = parse_all_colors(config, line);
	return (result);
}

int	parse_config_file(t_config *config, const char *path)
{
	int		fd;
	char	*line;
	int		result;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	line = get_next_line(fd);
	while (line != NULL)
	{
		result = parse_config_line(config, line);
		if (result < 0)
		{
			free(line);
			close(fd);
			return (-2);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
