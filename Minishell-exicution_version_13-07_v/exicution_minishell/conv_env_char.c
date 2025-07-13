/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_env_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:37:01 by maskour           #+#    #+#             */
/*   Updated: 2025/06/24 18:37:43 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_env(t_env *env)
{
	int i;
	t_env *current;

	i = 0;
	current = env;
	while (current)
	{
		i++;
		current = current->next;
	}
	return (i);
}
static void free_env_arr(char **env_arry, int i)
{
	while (--i >= 0)
				free(env_arry[i]);
	free(env_arry);
}

static char **empty_env_array(void)
{
	char **str;

	str = malloc(sizeof(char *) * 1);
	if (!str)
		return NULL;
	str[0] = NULL;
	return (str);
}

char **convert(t_env *env_list)
{
	int count;
	int i;
	char **env_arry;
	t_env *current;

	if (!env_list)
		return empty_env_array();
	count = count_env(env_list);
	env_arry = malloc (sizeof(char *) * (count + 1));
	if (!env_arry)
		return (NULL);
	current = env_list;
	i = -1;
	while (++i < count && current)
	{
		env_arry[i] = ft_strdup(current->data_env);
		if (!env_arry[i])
		{
			free_env_arr(env_arry, i);
			return (NULL);
		}
		current = current->next;
	}
	env_arry[count] = NULL;
	return (env_arry);
}
