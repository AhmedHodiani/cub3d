/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:27:25 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 20:25:48 by ataher           ###   ########.fr       */
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

void print_map(char **grid, int height)
{
	ft_printf("Map grid:\n");
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; grid[i][j]; j++)
		{
			if (grid[i][j] == '1')
				ft_printf("██");
			else if (grid[i][j] == '0')
				ft_printf("  ");
			else if (grid[i][j] == ' ')
				ft_printf("··");
			else
				ft_printf("%c ", grid[i][j]);
		}
		ft_printf("\n");
	}
}

int	parse_config_file(t_config *config, char *path)
{
	int		fd;
	int		result;
	size_t	len;

	len = ft_strlen(path);
	if (len < 4 || ft_strcmp(path + len - 4, ".cub") != 0)
		ft_exit(2, "Map file must have .cub extension");
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	result = read_config_lines(config, fd);
	close(fd);
	if (result == -1)
		ft_exit(3, "Failed to open map file");
	if (result == -2)
		ft_exit(4, "Invalid or duplicate identifier/s");
	if (result < 0)
		return (result);
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
