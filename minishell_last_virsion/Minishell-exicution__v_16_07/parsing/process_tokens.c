/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:17:31 by ahari             #+#    #+#             */
/*   Updated: 2025/07/14 05:10:10 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*process_quoted_section(char *val, int *i, t_env_list *env)
{
	char	quote;
	int		start;
	char	*tmp;
	char	*expanded;

	quote = val[(*i)++];
	start = *i;
	while (val[*i] && val[*i] != quote)
		(*i)++;
	tmp = ft_substr(val, start, *i - start);
	if (!tmp)
		return (NULL);
	if (quote == '\"')
	{
		expanded = found_env(tmp, env->env_table, env->exit_status);
		free(tmp);
		tmp = expanded;
	}
	if (val[*i] == quote)
		(*i)++;
	return (tmp);
}

static char	*process_unquoted_section(char *val, int *i, t_env_list *env)
{
	int		start;
	char	*tmp;
	char	*expanded;

	start = *i;
	while (val[*i] && val[*i] != '\'' && val[*i] != '\"')
		(*i)++;
	tmp = ft_substr(val, start, *i - start);
	if (!tmp)
		return (NULL);
	expanded = found_env(tmp, env->env_table, env->exit_status);
	free(tmp);
	tmp = expanded;
	if (ft_strcmp(tmp, "$") == 0)
	{
		free(tmp);
		tmp = ft_strdup("\1");
	}
	return (tmp);
}

static char	*init_result_buffer(char *val, t_token *head, size_t *capacity)
{
	char	*result;

	if (!val)
		return (print_error(head, NULL, NULL), NULL);
	*capacity = ft_strlen(val) + 1;
	result = malloc(*capacity);
	if (!result)
		return (print_error(head, NULL, NULL), NULL);
	result[0] = '\0';
	return (result);
}

char	*process_quoted_value_sections(char *val, t_token *head,
	t_env_list *env, char **temp)
{
	char	*result;
	char	*tmp;
	int		i;
	size_t	result_capacity;

	i = 0;
	if (ft_strchr(val, '\1') != NULL)
		return (remove_char(val, '\1'));
	result = init_result_buffer(val, head, &result_capacity);
	if (!result)
		return (NULL);
	while (val[i])
	{
		if (val[i] == '\'' || val[i] == '\"')
			tmp = process_quoted_section(val, &i, env);
		else
			tmp = process_unquoted_section(val, &i, env);
		if (!tmp)
			return (free(result), free(*temp), NULL);
		result = append_to_result(result, tmp, &result_capacity);
		if (!result)
			return (free(val), free(*temp), NULL);
	}
	return (result);
}

char	*process_quoted_value(char *val, t_token *head, t_env_list *env)
{
	char	*result;
	char	*temp;

	temp = NULL;
	if (ft_strchr(val, '=') != NULL)
		val = remove_dollar_before_quotes(val, &temp);
	if (!val)
		return (NULL);
	result = process_quoted_value_sections(val, head, env, &temp);
	if (!result)
		return (NULL);
	if (ft_strcmp(result, "\1") == 0)
	{
		free(result);
		result = ft_strdup("$");
	}
	return (free(temp), result);
}
