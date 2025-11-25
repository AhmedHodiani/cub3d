/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 23:26:56 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ft_exit(int code, const char *message)
{
	if (message && code != 0)
		ft_dprintf(2, RED "Error: %s\n" RESET, message);
	else if (message)
		ft_printf("%s\n", message);
	gc_clean();
	exit(code);
}

int	close_window(void *game)
{
	t_game	*g;
	int		i;

	g = (t_game *)game;
	if (g->mlx)
	{
		if (g->img.img)
			mlx_destroy_image(g->mlx, g->img.img);
		i = 0;
		while (i < 4)
		{
			if (g->textures[i].img.img)
				mlx_destroy_image(g->mlx, g->textures[i].img.img);
			i++;
		}
		if (g->win)
			mlx_destroy_window(g->mlx, g->win);
		mlx_destroy_display(g->mlx);
		free(g->mlx);
	}
	ft_exit(0, NULL);
	return (0);
}

void	*game_loop(void *game)
{
	t_game	*g;

	g = (t_game *)game;
	update_delta_time(g);
	process_movement(g);
	render_frame(g);
	cap_framerate(g);
	return (NULL);
}

void	init_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		ft_exit(1, "Failed to initialize MLX");
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	if (!game->win)
		ft_exit(1, "Failed to create window");
	game->img.img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (!game->img.img)
		ft_exit(1, "Failed to create image");
	game->img.addr = mlx_get_data_addr(game->img.img,
			&game->img.bits_per_pixel,
			&game->img.line_length, &game->img.endian);
}
