/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:01:26 by maskour           #+#    #+#             */
/*   Updated: 2024/11/20 17:56:48 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int	ft_printf(const char *format, ...);
int	ft_prints_single_character(int c);
int	ft_prints_unsigned_decimal(unsigned int nb);
int	ft_prints_decimal(int nb);
int	ft_prints_hexadecimal(unsigned int n, char c);
int	ft_prints_string(const char *s);
int	ft_prints_pointer(unsigned long nb);
#endif