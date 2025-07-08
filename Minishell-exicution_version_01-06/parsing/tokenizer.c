/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:04:38 by ahari             #+#    #+#             */
/*   Updated: 2025/07/05 16:36:36 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token *handle_word_with_quotes(char *str, int *i, t_token **head, t_shell *shell_ctx)
{
    int start = *i;
    int in_quotes = 0;
    char quote_type = 0;
    char *val = NULL;
    
    while (str[*i])
    {
        if (is_quote(str[*i]))
        {
            if (!in_quotes)
            {
                in_quotes = 1;
                quote_type = str[*i];
            }
            else if (str[*i] == quote_type)
            {
                in_quotes = 0;
                quote_type = 0;
            }
            (*i)++;
            continue ;
        }
        if (!in_quotes && (ft_isspace(str[*i]) || is_operator(str[*i])))
            break;
            
        (*i)++;
    }
    if (in_quotes)
    {
        print_error(*head, val, shell_ctx);
        return (NULL);
    }
    val = ft_strndup(&str[start], *i - start);
    if (!val)
        return (NULL);    
    t_token *new = new_token(val, TOKEN_WORD);
    if (has_quotes(new->value) == 1)
        new->was_quoted = 1;
    else if (has_quotes(new->value) == 3)
        new->was_quoted = 3;
    else
        new->was_quoted = 0;
    if (!new)
        return (free(val), NULL);
    add_token(head, new);
    return (*head);
}

t_token *handle_operator(char *str, int *i, t_token **head)
{
    char            *val;
    t_token_type    type;
    t_token         *new;

    val = NULL;
    if ((str[*i] == '>' || str[*i] == '<') && str[*i] == str[*i+1])
    {
        val = ft_strndup(&str[*i], 2);
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
    return (*head);
}
void ft_putstr(char *s)
{
    if (s)
        write(2, s, ft_strlen(s));
}
int check_syntax_errors(char *str, int i, t_shell *shell_ctx)
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

static int validate_syntax(char *str, t_shell *shell_ctx)
{
    int i = 0;
    char quote = 0;

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
        return 0;
    }
    return 1;
}

t_token	*string_tokens(char *str, t_shell *shell_ctx)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	if (!validate_syntax(str, shell_ctx))
		return (NULL);
	while (str[i])
	{
		while (ft_isspace(str[i]))
			i++;
		if (!str[i])
			break ;
		if (!check_syntax_errors(str, i, shell_ctx))
			return (NULL);
		if (is_operator(str[i]))
		{
			if (!handle_operator(str, &i, &head))
				return (free_tokens(head, NULL), NULL);
		}
		else
		{
			if (!handle_word_with_quotes(str, &i, &head, shell_ctx))
				return (free_tokens(head, NULL), NULL);
		}
	}
	return (head);
}

