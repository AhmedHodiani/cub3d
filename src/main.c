/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:45:08 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 13:22:43 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

void	ft_exit(int code, const char *message)
{
	if (message && code != 0)
		ft_dprintf(2, "Error: %s\n", message);
	else if (message)
		ft_printf("%s\n", message);
	gc_clean();
	exit(code);
}

static int	parse_texture(t_config *config, const char *line, 
					const char *prefix, char **texture, int flag)
{
	char	path[256];

	if (prefix[0] == 'N' && ft_sscanf(line, "NO %s", path) == 1)
	{
		if (config->config_flags & flag)
			return (-1);
		*texture = gc_strdup(path);
		if (!*texture)
			return (-1);
		config->config_flags |= flag;
		return (1);
	}
	else if (prefix[0] == 'S' && ft_sscanf(line, "SO %s", path) == 1)
	{
		if (config->config_flags & flag)
			return (-1);
		*texture = gc_strdup(path);
		if (!*texture)
			return (-1);
		config->config_flags |= flag;
		return (1);
	}
	return (0);
}

static int	parse_texture_we_ea(t_config *config, const char *line, 
					const char *prefix, char **texture, int flag)
{
	char	path[256];

	if (prefix[0] == 'W' && ft_sscanf(line, "WE %s", path) == 1)
	{
		if (config->config_flags & flag)
			return (-1);
		*texture = gc_strdup(path);
		if (!*texture)
			return (-1);
		config->config_flags |= flag;
		return (1);
	}
	else if (prefix[0] == 'E' && ft_sscanf(line, "EA %s", path) == 1)
	{
		if (config->config_flags & flag)
			return (-1);
		*texture = gc_strdup(path);
		if (!*texture)
			return (-1);
		config->config_flags |= flag;
		return (1);
	}
	return (0);
}

static int	parse_all_textures(t_config *config, const char *line)
{
	int	result;

	result = parse_texture(config, line, "NO", 
			&config->textures.north, FLAG_NO);
	if (result != 0)
		return (result);
	result = parse_texture(config, line, "SO", 
			&config->textures.south, FLAG_SO);
	if (result != 0)
		return (result);
	result = parse_texture_we_ea(config, line, "WE", 
			&config->textures.west, FLAG_WE);
	if (result != 0)
		return (result);
	result = parse_texture_we_ea(config, line, "EA", 
			&config->textures.east, FLAG_EA);
	return (result);
}

static int	validate_color(int r, int g, int b)
{
	if (r < 0 || r > 255)
		return (0);
	if (g < 0 || g > 255)
		return (0);
	if (b < 0 || b > 255)
		return (0);
	return (1);
}

static int	parse_color(t_config *config, const char *line, 
					const char *prefix, t_color *color, int flag)
{
	int		r, g, b;

	if (prefix[0] == 'F' && ft_sscanf(line, "F %d,%d,%d", &r, &g, &b) == 3)
	{
		if (!validate_color(r, g, b))
			return (-1);
		if (config->config_flags & flag)
			return (-1);
		*color = (t_color){r, g, b};
		config->config_flags |= flag;
		return (1);
	}
	else if (prefix[0] == 'C' && ft_sscanf(line, "C %d,%d,%d", &r, &g, &b) == 3)
	{
		if (!validate_color(r, g, b))
			return (-1);
		if (config->config_flags & flag)
			return (-1);
		*color = (t_color){r, g, b};
		config->config_flags |= flag;
		return (1);
	}
	return (0);
}

static int	parse_all_colors(t_config *config, const char *line)
{
	int	result;

	result = parse_color(config, line, "F", &config->floor, FLAG_F);
	if (result != 0)
		return (result);
	result = parse_color(config, line, "C", &config->ceiling, FLAG_C);
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

static int	parse_config_file(t_config *config, const char *path)
{
	int		fd;
	char	*line;
	int		result;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	while ((line = get_next_line(fd)) != NULL)
	{
		result = parse_config_line(config, line);
		if (result < 0)
		{
			free(line);
			close(fd);
			return (-2);
		}
		free(line);
	}
	close(fd);
	return (0);
}

static void	print_config(t_config *config)
{
	printf("North Texture: %s\n", config->textures.north);
	printf("South Texture: %s\n", config->textures.south);
	printf("West Texture: %s\n", config->textures.west);
	printf("East Texture: %s\n", config->textures.east);
	printf("Floor Color: R=%d, G=%d, B=%d\n", 
		config->floor.r, config->floor.g, config->floor.b);
	printf("Ceiling Color: R=%d, G=%d, B=%d\n", 
		config->ceiling.r, config->ceiling.g, config->ceiling.b);
}

int	main(void)
{
	t_config	config;
	char		*path;
	int			parse_result;

	if (gc_init() != 0)
		ft_exit(1, "Failed to initialize garbage collector");
	ft_memset(&config, 0, sizeof(t_config));
	path = "./maps/valid/cross_shape.cub";
	parse_result = parse_config_file(&config, path);
	if (parse_result == -1)
		ft_exit(1, "Failed to open map file");
	if (parse_result == -2)
		ft_exit(1, "Invalid or duplicate identifier/s");
	if ((config.config_flags & FLAG_ALL) != FLAG_ALL)
		ft_exit(1, "Missing identifier element/s");
	print_config(&config);
	ft_exit(0, NULL);
	return (0);
}