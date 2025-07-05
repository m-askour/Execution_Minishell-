/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 21:48:32 by ahari             #+#    #+#             */
/*   Updated: 2025/07/04 00:02:08 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_exit_status(t_shell *exit_stat)
{
	return (ft_itoa(exit_stat->exit_status));
}

int	is_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	ft_strspace(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (!ft_isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

char	*ft_delete_spaces(char *str)
{
	int	i;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '\0')
		return (NULL);
	return (ft_strdup(str + i));
}

int	get_var_name_length(char *str, int start)
{
	int	len;

	len = 0;
	while (str[start + len] && is_char(str[start + len]))
		len++;
	return (len);
}
