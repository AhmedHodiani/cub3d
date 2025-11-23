/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftsscanf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataher <ataher@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 19:05:48 by ataher            #+#    #+#             */
/*   Updated: 2025/11/23 12:57:03 by ataher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTSSCANF_H
# define LIBFTSSCANF_H

# include <stddef.h>
# include <stdarg.h>

/* Main sscanf implementation */
int			ft_sscanf(const char *str, const char *format, ...);

/* Utility functions */
int			sc_isspace(char c);
int			sc_isdigit(char c);
const char	*skip_spaces(const char *str);

/* Parsing functions */
int			parse_int(const char **str, int *result);
int			parse_string(const char **str, char *result);
int			parse_char(const char **str, char *result);

/* Format handling functions */
int			handle_format_d(const char **str, va_list args);
int			handle_format_s(const char **str, va_list args);
int			handle_format_c(const char **str, va_list args);
int			process_format(const char **format, const char **str,
				va_list args);

#endif
