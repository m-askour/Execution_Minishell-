/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   storge_tokens_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 05:41:41 by ahari             #+#    #+#             */
/*   Updated: 2025/07/14 06:00:10 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	validate_redirection_token(t_token *current)
{
	if (!current->next || current->next->type != TOKEN_WORD)
	{
		print_syntax_error(current);
		return (0);
	}
	return (1);
}

void	ft_putstr(char *s)
{
	if (s)
		write(2, s, ft_strlen(s));
}

int	check_syntax_errors(char *str, int i, t_shell *shell_ctx)
{
	if (str[0] == '|')
		return (write(2, "syntax error near unexpected token `|'\n", 40),
			shell_ctx->exit_status = 258, 0);
	else if (i >= 1 && str[i - 1] == '|' && str[i] == '|')
		return (write(2, "syntax error near unexpected token `|'\n", 40),
			shell_ctx->exit_status = 258, 0);
	else if (i >= 1 && str[i - 1] == ';' && str[i] == ';')
		return (write(2, "syntax error near unexpected token `;;'\n", 41),
			shell_ctx->exit_status = 258, 0);
	else if (str[i] == '\\')
		return (write(2, "syntax error near unexpected token `\\'\n", 40),
			shell_ctx->exit_status = 258, 0);
	else if (str[i] == ';')
		return (write(2, "syntax error near unexpected token `;'\n", 40),
			shell_ctx->exit_status = 258, 0);
	else if (str[i] == '!')
		return (write(2, "syntax error near unexpected token `!'\n", 40),
			shell_ctx->exit_status = 258, 0);
	else if (i >= 1 && str[i - 1] == ':' && str[i] == ':')
		return (write(2, "syntax error near unexpected token `::'\n", 41),
			shell_ctx->exit_status = 258, 0);
	else if (str[i] == '(' || str[i] == ')')
		return (write(2, "syntax error near unexpected token `()'\n", 41),
			shell_ctx->exit_status = 258, 0);
	return (1);
}

int	validate_syntax(char *str, t_shell *shell_ctx)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && !quote)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		i++;
	}
	if (quote)
	{
		write(2, "syntax error: unmatched quote\n", 31);
		shell_ctx->exit_status = 258;
		return (0);
	}
	return (1);
}

char	*extract_word_token(char *str, int start, int end)
{
	char	*val;

	val = ft_strndup(&str[start], end - start);
	if (!val)
		return (NULL);
	return (val);
}
