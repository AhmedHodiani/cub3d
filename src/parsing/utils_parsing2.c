/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:27:25 by ataher            #+#    #+#             */
/*   Updated: 2025/11/25 15:41:07 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	parse_texture_we(t_config *config, const char *line)
{
	char	path[256];

	if (ft_sscanf(line, "WE %s", path) == 1)
		return (set_texture(config, &config->textures.west, path, FLAG_WE));
	return (0);
}

int	parse_texture_ea(t_config *config, const char *line)
{
	char	path[256];

	if (ft_sscanf(line, "EA %s", path) == 1)
		return (set_texture(config, &config->textures.east, path, FLAG_EA));
	return (0);
}

int	set_color(t_config *config, t_color *color, int flag)
{
	(void)color;
	if (config->config_flags & flag)
		return (-1);
	config->config_flags |= flag;
	return (1);
}

int	parse_color_floor(t_config *config, const char *line)
{
	int	r;
	int	g;
	int	b;
	char c;

	c = '\0';
	if (ft_sscanf(line, "F %d,%d,%d %c", &r, &g, &b, &c) != 3)
		return (0);
	if (!validate_color(r, g, b))
		return (-1);
	config->floor = (t_color){r, g, b};
	return (set_color(config, &config->floor, FLAG_F));
}

int	parse_color_ceiling(t_config *config, const char *line)
{
	int	r;
	int	g;
	int	b;
	char c;

	c = '\0';
	if (ft_sscanf(line, "C %d,%d,%d %c", &r, &g, &b, &c) != 3)
		return (0);
	if (!validate_color(r, g, b))
		return (-1);
	config->ceiling = (t_color){r, g, b};
	return (set_color(config, &config->ceiling, FLAG_C));
}
