/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 22:43:07 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	load_texture(t_game *game, t_texture *texture, char *path)
{
	texture->img.img = mlx_xpm_file_to_image(game->mlx, path,
			&texture->width, &texture->height);
	if (!texture->img.img)
		ft_exit(1, "Failed to load texture");
	texture->img.addr = mlx_get_data_addr(texture->img.img,
			&texture->img.bits_per_pixel,
			&texture->img.line_length,
			&texture->img.endian);
}

void	load_textures(t_game *game)
{
	load_texture(game, &game->textures[NORTH], game->config.textures.north);
	load_texture(game, &game->textures[SOUTH], game->config.textures.south);
	load_texture(game, &game->textures[WEST], game->config.textures.west);
	load_texture(game, &game->textures[EAST], game->config.textures.east);
}

int	get_texture_color(t_texture *texture, int x, int y)
{
	char	*pixel;

	if (x < 0 || x >= texture->width || y < 0 || y >= texture->height)
		return (0);
	pixel = texture->img.addr + (y * texture->img.line_length
			+ x * (texture->img.bits_per_pixel / 8));
	return (*(int *)pixel);
}
