/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_config.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 23:26:56 by ataher           ###   ########.fr       */
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
