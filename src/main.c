/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:45:08 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 13:31:56 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

void	ft_exit(int code, const char *message)
{
	if (message && code != 0)
		ft_dprintf(2, "Error: %s\n", message);
	else if (message)
		ft_printf("%s\n", message);
	gc_clean();
	exit(code);
}

int	main(void)
{
	t_config	config;
	char		*path;
	int			parse_result;

	if (gc_init() != 0)
		ft_exit(1, "Failed to initialize garbage collector");
	ft_memset(&config, 0, sizeof(t_config));
	path = "./maps/valid/cross_shape.cub";
	parse_result = parse_config_file(&config, path);
	if (parse_result == -1)
		ft_exit(1, "Failed to open map file");
	if (parse_result == -2)
		ft_exit(1, "Invalid or duplicate identifier/s");
	if ((config.config_flags & FLAG_ALL) != FLAG_ALL)
		ft_exit(1, "Missing identifier element/s");
	print_config(&config);
	ft_exit(0, NULL);
	return (0);
}
