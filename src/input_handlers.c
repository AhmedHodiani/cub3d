/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 23:26:56 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	handle_keypress(int keycode, void *game)
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
