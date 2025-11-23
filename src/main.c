/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:45:08 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 21:30:27 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

void	ft_exit(int code, const char *message)
{
	if (message && code != 0)
		ft_dprintf(2, RED "Error: %s\n" RESET, message);
	else if (message)
		ft_printf("%s\n", message);
	gc_clean();
	exit(code);
}

int	close_window(t_game	*game)
{
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
	ft_exit(0, NULL);
	return (0);
}

int	handle_keypress(int keycode, void	*game)
{
	if (keycode == KEY_ESC)
		close_window((t_game *)game);
	return (0);
}

void	*game_loop(void	*game)
{
	(void)game;
	return (0);
}



static void	init_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		ft_exit(1, "Failed to initialize MLX");
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	if (!game->win)
		ft_exit(1, "Failed to create window");
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		ft_exit(1, "Usage: ./cub3D <map.cub>");
	if (gc_init() != 0)
		ft_exit(1, "Failed to initialize garbage collector");
	parse_config_file(&(game.config), argv[1]);
	
	
	init_mlx(&game);

	mlx_key_hook(game.win, handle_keypress, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);


	mlx_hook(game.win, 17, 0, &mlx_loop_end, game.mlx);
	mlx_loop(game.mlx);

	close_window(&game);
	ft_exit(0, "Game exited successfully");
	return (0);
}
