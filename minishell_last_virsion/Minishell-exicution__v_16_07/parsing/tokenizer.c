/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:04:38 by ahari             #+#    #+#             */
/*   Updated: 2025/07/14 06:07:53 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	parse_word_boundaries(char *str, int *i,
	int *in_quotes, char *quote_type)
{
	while (str[*i])
	{
		if (is_quote(str[*i]))
		{
			process_quote_parsing(str, i, quote_type, in_quotes);
			continue ;
		}
		if (!*in_quotes && (ft_isspace(str[*i]) || is_operator(str[*i])))
			break ;
		(*i)++;
	}
	return (*in_quotes);
}

t_token	*handle_word_with_quotes(char *str, int *i,
	t_token **head, t_shell *shell_ctx)
{
	int		start;
	int		in_quotes;
	char	quote_type;
	char	*val;
	t_token	*result;

	start = *i;
	in_quotes = 0;
	quote_type = 0;
	if (parse_word_boundaries(str, i, &in_quotes, &quote_type))
		return (print_error(*head, NULL, shell_ctx), NULL);
	val = extract_word_token(str, start, *i);
	if (!val)
		return (NULL);
	result = create_and_add_token(val, head);
	return (free(val), result);
}

t_token	*handle_operator(char *str, int *i, t_token **head)
{
	char			*val;
	t_token_type	type;
	t_token			*new;

	val = NULL;
	if ((str[*i] == '>' || str[*i] == '<') && str[*i] == str[*i + 1])
	{
		val = ft_strndup(&str[*i], 2);
		if (!val)
			return (NULL);
		*i += 2;
	}
	else
		val = ft_strndup(&str[(*i)++], 1);
	if (!val)
		return (NULL);
	type = get_token_type(val);
	new = new_token(val, type);
	if (!new)
		return (free(val), NULL);
	add_token(head, new);
	return (free(val), *head);
}

static t_token	*process_token_logic(char *str, int *i,
	t_token **head, t_shell *shell_ctx)
{
	if (!check_syntax_errors(str, *i, shell_ctx))
		return (free_tokens(*head, NULL), NULL);
	if (is_operator(str[*i]))
	{
		if (!handle_operator(str, i, head))
			return (free_tokens(*head, NULL), NULL);
	}
	else
	{
		if (!handle_word_with_quotes(str, i, head, shell_ctx))
			return (free_tokens(*head, NULL), NULL);
	}
	return (*head);
}

t_token	*string_tokens(char *str, t_shell *shell_ctx)
{
	t_token	*head;
	int		i;

	i = 0;
	head = NULL;
	if (!validate_syntax(str, shell_ctx))
		return (NULL);
	while (str[i])
	{
		while (ft_isspace(str[i]))
			i++;
		if (!str[i])
			break ;
		if (!process_token_logic(str, &i, &head, shell_ctx))
			return (NULL);
	}
	return (head);
}
