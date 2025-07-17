/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 21:48:32 by ahari             #+#    #+#             */
/*   Updated: 2025/07/14 03:50:35 by ahari            ###   ########.fr       */
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
	int		start;
	int		end;
	char	*result;

	if (!str)
		return (NULL);
	start = 0;
	while (str[start] && ft_isspace(str[start]))
		start++;
	if (str[start] == '\0')
		return (NULL);
	end = ft_strlen(str) - 1;
	while (end >= start && ft_isspace(str[end]))
		end--;
	result = ft_substr(str, start, end - start + 1);
	if (!result)
		return (NULL);
	return (result);
}

int	get_var_name_length(char *str, int start)
{
	int	len;

	len = 0;
	while (str[start + len] && is_char(str[start + len]))
		len++;
	return (len);
}
