/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 23:26:56 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	print_map_char(char c)
{
	if (c == '1')
		ft_printf("██");
	else if (c == '0')
		ft_printf("  ");
	else if (c == ' ')
		ft_printf("··");
	else
		ft_printf("%c ", c);
}

void	print_map(char **grid, int height)
{
	int	i;
	int	j;

	ft_printf("Map grid:\n");
	i = 0;
	while (i < height)
	{
		j = 0;
		while (grid[i][j])
		{
			print_map_char(grid[i][j]);
			j++;
		}
		ft_printf("\n");
		i++;
	}
}
