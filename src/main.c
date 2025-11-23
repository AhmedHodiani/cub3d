/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:45:08 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 22:23:46 by ataher           ###   ########.fr       */
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

int	close_window(void *game)
{
	t_game	*g;

	g = (t_game *)game;
	if (g->win)
		mlx_destroy_window(g->mlx, g->win);
	if (g->mlx)
	{
		mlx_destroy_display(g->mlx);
		free(g->mlx);
	}
	ft_exit(0, NULL);
	return (0);
}

int	handle_keypress(int keycode, void	*game)
{
	t_game	*g;

	g = (t_game *)game;
	if (keycode == KEY_ESC)
		close_window(g);
	if (keycode == KEY_W)
		g->keys.w = 1;
	if (keycode == KEY_A)
		g->keys.a = 1;
	if (keycode == KEY_S)
		g->keys.s = 1;
	if (keycode == KEY_D)
		g->keys.d = 1;
	if (keycode == KEY_LEFT)
		g->keys.left = 1;
	if (keycode == KEY_RIGHT)
		g->keys.right = 1;
	return (0);
}

int	handle_keyrelease(int keycode, void *game)
{
	t_game	*g;

	g = (t_game *)game;
	if (keycode == KEY_W)
		g->keys.w = 0;
	if (keycode == KEY_A)
		g->keys.a = 0;
	if (keycode == KEY_S)
		g->keys.s = 0;
	if (keycode == KEY_D)
		g->keys.d = 0;
	if (keycode == KEY_LEFT)
		g->keys.left = 0;
	if (keycode == KEY_RIGHT)
		g->keys.right = 0;
	return (0);
}

void	*game_loop(void	*game)
{
	t_game	*g;

	g = (t_game *)game;
	update_delta_time(g);
	process_movement(g);
	render_frame(g);
	cap_framerate(g);
	return (NULL);
}



static void	init_mlx(t_game *game)
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
	init_player(&game);
	game.time.last_frame = get_time_us();
	game.time.delta_time = 0.016;

	mlx_hook(game.win, 2, 1L << 0, (int (*)())handle_keypress, &game);
	mlx_hook(game.win, 3, 1L << 1, (int (*)())handle_keyrelease, &game);
	mlx_hook(game.win, 17, 0, (int (*)())close_window, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);

	mlx_loop(game.mlx);

	close_window(&game);
	ft_exit(0, "Game exited successfully");
	return (0);
}
