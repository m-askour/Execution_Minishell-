/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 05:06:46 by ahari             #+#    #+#             */
/*   Updated: 2025/07/14 06:10:08 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*realloc_result(char *result, size_t *capacity, size_t new_len)
{
	char	*new_result;
	size_t	old_capacity;

	if (new_len <= *capacity)
		return (result);
	old_capacity = *capacity;
	*capacity = new_len * 2;
	new_result = ft_realloc(result, old_capacity, *capacity);
	if (!new_result)
	{
		free(result);
		return (NULL);
	}
	return (new_result);
}

char	*append_to_result(char *result, char *tmp, size_t *capacity)
{
	size_t	result_len;
	char	*new_result;

	if (!tmp)
		return (result);
	result_len = ft_strlen(result) + ft_strlen(tmp) + 1;
	new_result = realloc_result(result, capacity, result_len);
	if (!new_result)
		return (free(tmp), free(result), NULL);
	ft_strcat(new_result, tmp);
	free(tmp);
	return (new_result);
}

void	free_cmd_args(char **args, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	print_syntax_error(t_token *current)
{
	printf("minishell: syntax error near unexpected token `");
	if (current->next && current->next->value)
		printf("%s", current->next->value);
	else
		printf("newline");
	printf("'\n");
}

int	count_commands(t_token *tokens)
{
	int	count;

	count = 1;
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
			count++;
		tokens = tokens->next;
	}
	return (count);
}
