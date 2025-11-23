/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:45:08 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 23:26:56 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	setup_hooks(t_game *game)
{
	mlx_hook(game->win, 2, 1L << 0, (int (*)())handle_keypress, game);
	mlx_hook(game->win, 3, 1L << 1, (int (*)())handle_keyrelease, game);
	mlx_hook(game->win, 17, 0, (int (*)())close_window, game);
	mlx_loop_hook(game->mlx, game_loop, game);
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		ft_exit(1, "Usage: ./cub3D <map.cub>");
	if (gc_init() != 0)
		ft_exit(1, "Failed to initialize garbage collector");
	ft_memset(&game, 0, sizeof(t_game));
	parse_config_file(&(game.config), argv[1]);
	init_mlx(&game);
	load_textures(&game);
	init_player(&game);
	game.time.last_frame = get_time_us();
	game.time.delta_time = 0.016;
	setup_hooks(&game);
	mlx_loop(game.mlx);
	close_window(&game);
	ft_exit(0, "Game exited successfully");
	return (0);
}
