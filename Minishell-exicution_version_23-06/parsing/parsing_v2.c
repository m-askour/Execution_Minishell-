/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_v2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 23:15:00 by ahari             #+#    #+#             */
/*   Updated: 2025/06/29 23:12:29 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"


static void	process_export_assignment_split(char **new_val, int i, int is_export_var)
{
	char	**split;
	char	*joined;
    char    *tmp;

	split = split_with_quotes(new_val[i]);
	if (!split || !split[0] || !split[1])
	{
		if (split)
			free_array(split);
		return ;
	}
	if (is_export_var == 1)
	{
		joined = join_export_tokens(split);
		if (joined)
		{
			free(new_val[i]);
			new_val[i] = joined;
		}
		free_array(split);
	}
	else if (is_export_var == 2)
	{
		tmp = split[0];
		split[0] = NULL;
		free_array(split);
		free(new_val[i]);
		new_val[i] = tmp;
	}
}


static int	finalize_token_value(t_token *current, t_token **head, char *val_cmd, char **new_val)
{
	free_array(new_val);
	free(current->value);
	if (val_cmd)
		current->value = val_cmd;
	else
		current->value = ft_strdup("");
	if (!current->value)
	{
		free_tokens(*head, NULL);
		return (0);
	}
	return (1);
}

int	join_new_value(char **val_cmd, char *str, t_token **head, char **new_val)
{
	char	*tmp;

	if (!*val_cmd)
	{
		*val_cmd = ft_strdup(str);
		if (!*val_cmd)
			return (free_array(new_val), free_tokens(*head, NULL), 0);
	}
	else
	{
		tmp = ft_strjoin(*val_cmd, str);
		free(*val_cmd);
		*val_cmd = tmp;
		if (!*val_cmd)
			return (free_array(new_val), free_tokens(*head, NULL), 0);
	}
	return (1);
}

int	handle_split(t_token *cur, t_token **head,
		char **new_val, int i)
{
	char	**split;

	split = split_with_quotes(new_val[i]);
	if (split && split[0] && split[1])
	{
		if (!handle_token_splitting(cur, head, split))
			return (free_tokens(*head, NULL), free_array(new_val),
				free_array(split), 0);
		return (free_array(split), free_array(new_val), 2);
	}
	if (split)
		free_array(split);
	return (1);
}
int	process_token_loop(t_token *cur, t_token **head,
		t_shell *ctx, t_process p)
{
	int	i;
	int	ret;

	i = 0;
	while (p.new_val[i])
	{
		if (!is_single_quoted(cur->value, p.new_val[i]))
		{
			 if (p.new_val[i][0] == '\1')
            {
                char *unmarked = ft_strdup(p.new_val[i] + 1);
                if (!unmarked)
                    return 0;
                char *expanded = found_env(unmarked, p.env, ctx);
                free(unmarked);
                if (!expanded)
                    return 0;
                free(p.new_val[i]);
                p.new_val[i] = expanded;
                if (!join_new_value(&p.val_cmd, p.new_val[i], head, p.new_val))
                    return 0;
                i++;
                continue;
            }
			if (!process_env_expansion(p.new_val, i, p.env, ctx))
				return (free_tokens(*head, NULL), free_array(p.new_val),
					free(p.val_cmd), 0);
			if (!is_quoted(cur->value, p.new_val[i]) && !p.is_export)
			{
				ret = handle_split(cur, head, p.new_val, i);
				if (ret != 1)
					return (free(p.val_cmd), ret);
			}
			else if (p.is_export && !is_quoted(cur->value, p.new_val[i]))
				process_export_assignment_split(p.new_val, i, p.is_export);
		}
		if (!join_new_value(&p.val_cmd, p.new_val[i], head, p.new_val))
			return (0);
		i++;
	}
	return (finalize_token_value(cur, head, p.val_cmd, p.new_val));
}

int	process_token(t_token *current, t_token **head,
		t_shell *shell_ctx, char **env_table)
{
	char	**new_val;
	char	*val_cmd;
	int		is_export_var;

	if (current->type != TOKEN_WORD)
		return (1);
	is_export_var = is_export_assignment(*head, current);
	new_val = process_quoted_value(current->value, *head, shell_ctx);
	if (!new_val)
		return (0);
	val_cmd = NULL;
	return (process_token_loop(current, head, shell_ctx,
			(t_process){new_val, val_cmd, env_table, is_export_var}));
}