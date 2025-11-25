/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:27:25 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 18:36:11 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	validate_color(int r, int g, int b)
{
	if (r < 0 || r > 255)
		return (0);
	if (g < 0 || g > 255)
		return (0);
	if (b < 0 || b > 255)
		return (0);
	return (1);
}

int	is_xpm_file(const char *path)
{
	int	len;

	len = ft_strlen(path);
	if (len < 4)
		return (0);
	if (ft_strncmp(path + len - 4, ".xpm", 4) != 0)
		return (0);
	return (1);
}

void	print_config(t_config *config)
{
	printf("North Texture: %s\n", config->textures.north);
	printf("South Texture: %s\n", config->textures.south);
	printf("West Texture: %s\n", config->textures.west);
	printf("East Texture: %s\n", config->textures.east);
	printf("Floor Color: R=%d, G=%d, B=%d\n",
		config->floor.r, config->floor.g, config->floor.b);
	printf("Ceiling Color: R=%d, G=%d, B=%d\n",
		config->ceiling.r, config->ceiling.g, config->ceiling.b);
	ft_printf("Map: %dx%d, Player at (%d, %d)\n",
		config->map.width, config->map.height,
		config->map.player_pos[0], config->map.player_pos[1]);
}

int	set_texture(t_config *config, char **texture, char *path, int flag)
{
	int	fd;

	if (config->config_flags & flag)
		return (-1);
	if (!is_xpm_file(path))
		return (-1);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	close(fd);
	*texture = gc_strdup(path);
	if (!*texture)
		return (-1);
	config->config_flags |= flag;
	return (1);
}

int	parse_texture_no(t_config *config, const char *line)
{
	char	path[256];

	if (ft_sscanf(line, "NO %s", path) == 1)
		return (set_texture(config, &config->textures.north, path, FLAG_NO));
	return (0);
}

int	parse_texture_so(t_config *config, const char *line)
{
	char	path[256];

	if (ft_sscanf(line, "SO %s", path) == 1)
		return (set_texture(config, &config->textures.south, path, FLAG_SO));
	return (0);
}
