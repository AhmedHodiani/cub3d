/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:45:08 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 18:36:11 by ataher           ###   ########.fr       */
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

void print_map(char **grid, int height)
{
	ft_printf("Map grid:\n");
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; grid[i][j]; j++)
		{
			if (grid[i][j] == '1')
				ft_printf("██");
			else if (grid[i][j] == '0')
				ft_printf("  ");
			else if (grid[i][j] == ' ')
				ft_printf("··");
			else
				ft_printf("%c ", grid[i][j]);
		}
		ft_printf("\n");
	}
}

int	main(int argc, char **argv)
{
	t_config	config;
	size_t		len;

	if (argc != 2)
		ft_exit(1, "Usage: ./cub3D <map.cub>");
	len = ft_strlen(argv[1]);
	if (len < 4 || ft_strcmp(argv[1] + len - 4, ".cub") != 0)
		ft_exit(1, "Map file must have .cub extension");
	if (gc_init() != 0)
		ft_exit(1, "Failed to initialize garbage collector");
	ft_memset(&config, 0, sizeof(t_config));
	parse_config_file(&config, argv[1]);

	print_config(&config);
	print_map(config.map.grid, config.map.height);

	if (validate_map_walls(&config) < 0)
		ft_exit(1, "Map not properly closed by walls");


	ft_exit(0, NULL);
	return (0);
}
