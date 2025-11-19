/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:35:43 by ataher            #+#    #+#             */
/*   Updated: 2025/01/04 05:49:11 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H

# include <unistd.h>
# include <stdarg.h>
# include <stdlib.h>
# include <stdio.h>

# define RESET "\033[0m"
# define BLACK "\033[30m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"
# define GRAY "\033[90m"
# define BRIGHT_RED "\033[91m"
# define BRIGHT_GREEN "\033[92m"
# define BRIGHT_YELLOW "\033[93m"
# define BRIGHT_BLUE "\033[94m"
# define BRIGHT_MAGENTA "\033[95m"
# define BRIGHT_CYAN "\033[96m"
# define BRIGHT_WHITE "\033[97m"

int		ft_printf(const char *format, ...);
int		ft_dprintf(int fd, const char *format, ...);
int		ft_color_printf(char *color, const char *format, ...);

int		hp_printf(int fd, char format, va_list inputs);
int		ft_putaddress_fd(void *address, int fd);
int		ft_putnbr_fd(int n, int fd);
int		ft_putstr_fd(char *s, int fd);
int		ft_puthex_fd(unsigned int num, const char format, int fd);
int		ft_putunsigned_fd(unsigned int n, int fd);

// helpers
void	hp_puthex_fd(unsigned long num, const char format, int fd);
int		hp_putchar_fd(char c, int fd);
size_t	hp_hexlen(unsigned	long num);
size_t	hp_strlen(char const *s);

// buggers
void	bugger_cmd(char *format, ...);
void	bugger_info(char *format, ...);
void	bugger_error(char *format, ...);
void	bugger_box(char *format, ...);
void	bugger_cute(char *format, ...);
void	bugger_warning(char *format, ...);

#endif
