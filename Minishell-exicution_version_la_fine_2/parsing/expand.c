/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:43:32 by ahari             #+#    #+#             */
/*   Updated: 2025/07/01 21:46:54 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *get_env(char **env, char *found_env)
{
    int		i;
    char	*env_value;

	i = 0;
    if (!env || !found_env)
        return NULL;
    while (env[i])
    {
        if (ft_strncmp(env[i], found_env, ft_strlen(found_env)) == 0 &&
            env[i][ft_strlen(found_env)] == '=')
        {
            env_value = ft_strdup(env[i] + ft_strlen(found_env) + 1);
            return env_value;
        }
        i++;
    }
    return NULL;
}

static char *ft_exit_status(t_shell *exit_stat)
{
    return ft_itoa(exit_stat->exit_status);
}

static int is_char(char c)
{
    return (ft_isalnum(c) || c == '_');
}

static int get_var_name_length(char *str, int start)
{
    int	len;

	len = 0;
    while (str[start + len] && is_char(str[start + len]))
        len++;
    return len;
}

static char *build_new_command(char *cmd, int pos, char *replacement, int skip_len)
{
    char	*new_cmd;
    int		new_len;

	new_len = ft_strlen(cmd) - skip_len + ft_strlen(replacement);
    new_cmd = malloc(new_len + 1);
    if (!new_cmd)
        return NULL;
    ft_strncpy(new_cmd, cmd, pos);
    new_cmd[pos] = '\0';
    ft_strcat(new_cmd, replacement);
    ft_strcat(new_cmd, cmd + pos + skip_len);
    return new_cmd;
}

static char *handle_special_var(char *cmd, int pos, t_shell *exited)
{
    char	*replacement;
    char	*new_cmd;

	replacement = NULL;
    if (cmd[pos + 1] == '?')
        replacement = ft_exit_status(exited);
    else if (cmd[pos + 1] == '0')
        replacement = ft_strdup("./minishell");
    else if (ft_isdigit(cmd[pos + 1]))
        replacement = ft_strdup("");
    else
        return (NULL);
    if (!replacement)
        return (NULL);
    new_cmd = build_new_command(cmd, pos, replacement, 2);
    free(replacement);
    return new_cmd;
}
int ft_strspace(char *str)
{
    int i = 0;
    if (!str)
        return 0;
    while (str[i])
    {
        if (!ft_isspace(str[i]))
            return 0;
        i++;
    }
    return 1;
}

char *ft_delete_spaces(char *str)
{
    if (!str)
        return NULL;
    int i= 0;
    while (str[i] && ft_isspace(str[i]))
    {
        i++;
    }
    if (str[i] == '\0')
        return NULL;
    return (ft_strdup(str + i));
    
}
static char *handle_env_var(char *cmd, int pos, char **env)
{
    int		var_start;
    int		var_len;
    char	*env_name;
	char	*env_value;
	char	*new_cmd;
    
	var_start = pos + 1;
	var_len = get_var_name_length(cmd, var_start);
    if (var_len == 0)
        return (NULL);
    env_name = ft_substr(cmd, var_start, var_len);
    if (!env_name)
        return (NULL);
    env_value = get_env(env, env_name);
    if (ft_strspace(env_value) == 1)
        env_value = ft_strdup(" ");
    else
        env_value = ft_delete_spaces(env_value);
    free(env_name);
    if (env_value != NULL && ft_strspace(env_value) != 1)
    {
        new_cmd = build_new_command(cmd, pos, env_value, var_len + 1);
        free(env_value);
    }
    else
        new_cmd = build_new_command(cmd, pos, "\2", var_len + 1);
    return (new_cmd);
}

static char *replace_double_dollar(char *cmd)
{
    int     len = ft_strlen(cmd);
    char    *new_cmd = malloc(len + 1);
    int     i = 0;
    int     j = 0;
    if (!new_cmd)
        return NULL;
    while (cmd[i]) {
        if (cmd[i] == '$' && cmd[i + 1] == '$')
        {
            new_cmd[j++] = '\x01';
            i += 2;
        }
        else
            new_cmd[j++] = cmd[i++];
    }
    new_cmd[j] = '\0';
    return new_cmd;
}

static int	should_expand_var(char *cmd, int pos)
{
	if (cmd[pos] != '$')
		return (0);
	if (pos > 0 && cmd[pos - 1] == '\\')
		return (0);
	return (1);
}

static char *process_special_variable(char *cmd, int pos, t_shell *shell_ctx)
{
    char *new_cmd;
    char *expanded;

    if (cmd[pos + 1] == '?' || cmd[pos + 1] == '0' || ft_isdigit(cmd[pos + 1]))
    {
        new_cmd = handle_special_var(cmd, pos, shell_ctx);
        if (new_cmd)
        {
            expanded = found_env(new_cmd, NULL, shell_ctx);
            free(new_cmd);
            return expanded;
        }
    }
    return NULL;
}


static char *process_env_variable(char *cmd, int pos, char **env, t_shell *shell_ctx)
{
    char *new_cmd;
    char *expanded;

    if (cmd[pos + 1] && is_char(cmd[pos + 1]))
    {
        new_cmd = handle_env_var(cmd, pos, env);
        if (new_cmd)
        {
            expanded = found_env(new_cmd, env, shell_ctx);
            free(new_cmd);
            return expanded;
        }
    }
    return NULL;
}


static char	*restore_dollar_signs(char *cmd)
{
	int		i;
	char	*result;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\x01')
			cmd[i] = '$';
		i++;
	}
	result = ft_strdup(cmd);
	return (result);
}

static char	*process_variables(char *cmd, char **env, t_shell *shell_ctx)
{
	int		pos;
	char	*result;

	pos = 0;
	while (cmd[pos])
	{
		if (should_expand_var(cmd, pos))
		{
			result = process_special_variable(cmd, pos, shell_ctx);
			if (result)
				return (result);
			result = process_env_variable(cmd, pos, env, shell_ctx);
			if (result)
				return (result);
		}
		pos++;
	}
	return (restore_dollar_signs(cmd));
}

char *found_env(char *cmd, char **env, t_shell *shell_ctx)
{
    char    *preprocessed;
    char    *result;
    char    *expanded;

    if (!cmd)
        return (NULL);
    if (cmd[0] == '\1')
    {
        expanded = process_variables(cmd + 1, env, shell_ctx);
        if (!expanded)
            return (NULL);
        return expanded;
    }
    preprocessed = replace_double_dollar(cmd);
    if (!preprocessed)
        return (NULL);
    result = process_variables(preprocessed, env, shell_ctx);
    free(preprocessed);
    return (result);
}

