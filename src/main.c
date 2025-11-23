/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:45:08 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 20:26:06 by ataher           ###   ########.fr       */
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



int	main(int argc, char **argv)
{
	t_config	config;

	if (argc != 2)
		ft_exit(1, "Usage: ./cub3D <map.cub>");

	if (gc_init() != 0)
		ft_exit(1, "Failed to initialize garbage collector");

	ft_memset(&config, 0, sizeof(t_config));
	parse_config_file(&config, argv[1]);


	




	ft_exit(0, NULL);
	return (0);
}
