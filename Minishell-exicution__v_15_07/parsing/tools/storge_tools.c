/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   storge_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 05:40:02 by ahari             #+#    #+#             */
/*   Updated: 2025/07/14 06:11:37 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*remove_char(const char *str, char to_remove)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	result = malloc(strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] != to_remove)
		{
			result[j++] = str[i];
		}
		i++;
	}
	result[j] = '\0';
	return (result);
}

int	count_redirections(t_token *start)
{
	int		count;
	t_token	*current;

	count = 0;
	current = start;
	while (current && current->type != TOKEN_PIPE)
	{
		if (ft_isredirect(current->type))
			count++;
		current = current->next;
	}
	return (count);
}

int	process_quote_parsing(char *str, int *i, char *quote_type, int *in_quotes)
{
	if (!*in_quotes)
	{
		*in_quotes = 1;
		*quote_type = str[*i];
	}
	else if (str[*i] == *quote_type)
	{
		*in_quotes = 0;
		*quote_type = 0;
	}
	(*i)++;
	return (1);
}

t_token	*create_and_add_token(char *val, t_token **head)
{
	t_token	*new;

	new = new_token(val, TOKEN_WORD);
	if (!new)
		return (NULL);
	if (has_quotes(new->value) == 1 || has_quotes(new->value) == 2)
		new->was_quoted = 1;
	else
		new->was_quoted = 0;
	add_token(head, new);
	return (*head);
}
