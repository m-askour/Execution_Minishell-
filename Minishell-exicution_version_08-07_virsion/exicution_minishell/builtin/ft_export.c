/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:50:12 by maskour           #+#    #+#             */
/*   Updated: 2025/07/11 22:51:09 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	print_sort_env(t_env *env)
{
	t_env	*current;
	char	*eq;
	int		key_len;

	current = env;
	while (current)
	{
		eq = ft_strchr(current->data_env, '=');
		if (eq)
		{
			key_len = eq - current->data_env;
			printf("declare -x ");
			fwrite(current->data_env, 1, key_len, stdout);
			printf("=\"%s\"\n", eq + 1);
		} 
		else
			printf("declare -x %s\n", current->data_env);
		current = current->next;
	}
}

static t_env	*sort_env(t_env *env)
{
	int		swapped;
	t_env	*ptr1;
	t_env	*lptr;
	char	*tmp;

	if (!env || !env->next) 
		return env;
	lptr = NULL;
	while (swapped) 
	{
		swapped = 0;
		ptr1 = env;
		while (ptr1->next != lptr) {
			if (ft_strcmp(ptr1->data_env, ptr1->next->data_env) > 0)
			{
				tmp = ptr1->data_env;
				ptr1->data_env = ptr1->next->data_env;
				ptr1->next->data_env = tmp;
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	}
	return (env);
}

static void	sort_and_display_env(t_env **envp)
{
	*envp = sort_env(*envp);
	print_sort_env(*envp);
}

static int	is_valid_key(char *key)
{
	int	i;

	if (!key || !*key)
		return (0);
	if (!ft_isalpha_up(*key) && *key != '_')
		return 0;
	i = 0;
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return 0;
	}
	return (1);
}

static int	is_exist(t_env *env, char *key)
{
	t_env	*current;
	int		key_len;

	if (!key)
		return (0);
	current = env;
	key_len = ft_strlen(key);
	while (current)
	{
		if (!ft_strncmp(current->data_env, key, key_len) &&
			(current->data_env[key_len] == '=' || current->data_env[key_len] == '\0'))
			return (1);
		current = current->next;
	}
	return (0);
}

static void extra_key_value(char *input, char **key, char **value)
{
	*key = NULL;
	*value = NULL;
	if (!input)
		return;

	char *input_copy = ft_strdup(input);
	if (!input_copy)
		return;

	char *equal = ft_strchr(input_copy, '=');
	if (equal) {
		if (equal > input_copy && *(equal - 1) == '+') {
			*(equal - 1) = '\0';
		} else {
			*equal = '\0';
		}
		*key = ft_strdup(input_copy);
		*value = ft_strdup(equal + 1);
	} else {
		*key = ft_strdup(input_copy);
		*value = NULL;
	}
	free(input_copy);
}

static void append_env_node(t_env **env, t_env *new_node)
{
	if (!*env)
	{
		*env = new_node;
		return;
	}
	t_env *last = *env;
	while (last->next)
		last = last->next;
	last->next = new_node;
}

static int update_exist_env(t_env *env, char *key, char *value)
{
	t_env *current = env;

	while (current)
	{
		if (!strncmp(current->data_env, key, strlen(key)) &&
			(current->data_env[strlen(key)] == '=' || current->data_env[strlen(key)] == '\0')) 
		{
			free(current->data_env);
			if (value)
			{
				char *tmp = ft_strjoin(key, "=");
				if (!tmp) 
					return 1;
				current->data_env = ft_strjoin(tmp, value);
				free(tmp);
			}
			else 
				current->data_env = strdup(key);
			return 1; 
		}
		current = current->next;
	}
	return 0;
}

static char	*merge_key_value(char *key, char *value)
{
	char *tmp;
	char *result;

	if (value)
	{
		tmp = ft_strjoin(key, "=");
		if (!tmp)
			return NULL;
		result = ft_strjoin(tmp, value);
		free(tmp);
		if (!result)
			return NULL;
		return result;
	}
	return ft_strdup(key);
}

static void	add_env_export(t_env **env, char *key, char *value)
{
	t_env *new_node;

	if (update_exist_env(*env, key, value))
		return;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return;
	new_node->data_env = merge_key_value(key, value);
	if (!new_node->data_env)
	{
		free(new_node);
		return;
	}
	new_node->next = NULL;
	append_env_node(env, new_node);
}

static void update_or_add_env(t_env **env, char *key, char *value)
{
	t_env *current;
	int key_len;
	char *tmp;
	char *add_equal;

	if (!key) return;
	key_len = ft_strlen(key);
	current = *env;
	while (current)
	{
		if (!ft_strncmp(current->data_env, key, key_len) && current->data_env[key_len] == '=')
		{
			tmp = ft_strjoin(key, "=");
			if (!tmp) return;
			add_equal = ft_strjoin(tmp, value);
			free(tmp);
			if (!add_equal) return;
			free(current->data_env);
			current->data_env = add_equal;
			return;
		}
		current = current->next;
	}
	add_env_export(env, key, value);
}

static char *build_appended_env_value(char *key, char *old_val, char *new_val)
{
	char *joined;
	char *prefix;
	char *final_val;

	joined = ft_strjoin(old_val, new_val);
	if (!joined)
		return NULL;
	prefix = ft_strjoin(key, "=");
	if (!prefix) {
		free(joined);
		return NULL;
	}
	final_val = ft_strjoin(prefix, joined);
	free(prefix);
	free(joined);
	return final_val;
}

static int update_env_value(t_env *current, char *key, char *value)
{
	char *old_val;
	char *new_data;

	old_val = ft_strchr(current->data_env, '=');
	if (!old_val)
		return (0);
	old_val++;
	new_data = build_appended_env_value(key, old_val, value);
	if (!new_data)
		return (0);
	free(current->data_env);
	current->data_env = new_data;
	return (1);
}

static void append_value(t_env **env, char *key, char *value)
{
	t_env *current;
	int key_len;

	if (!key || !value)
		return ;
	current = *env;
	key_len = ft_strlen(key);
	while (current)
	{
		if (!ft_strncmp(current->data_env, key, key_len) && current->data_env[key_len] == '=')
		{
			if (update_env_value(current, key, value))
				return;
			else
				return;
		}
		current = current->next;
	}
	add_env_export(env, key, value);
}

static void handel_append(t_env **env, char *cmd)
{
	char *key;
	char *value;

	key = NULL;
	value = NULL;
	extra_key_value(cmd, &key, &value);
	if (!is_valid_key(key))
	{
		ft_putstr_fd_up("minishell: export: ", 2);
		ft_putstr_fd_up(cmd, 2);
		perror(": not a valid identifier1\n");
		free(key); free(value);
		return;
	}
	append_value(env, key, value);
	free(key); free(value);
}

static void handle_assigmnet(t_env **env, char *cmd)
{
	char *key;
	char *value;

	key = NULL;
	value = NULL;
	extra_key_value(cmd, &key, &value);
	if (!is_valid_key(key))
	{
		ft_putstr_fd_up("minishell: export: ", 2);
		ft_putstr_fd_up(cmd, 2);
		perror(": not a valid identifier2\n");
		free(key); free(value);
		return;
	}
	update_or_add_env(env, key, value);
	free(key); free(value);
}

static void handle_export(t_env **env, char *cmd)
{
	char *empty;
	char *key;

	empty = "";
	key = NULL;
	extra_key_value(cmd, &key, &empty);
	if (!is_valid_key(key)) {
		ft_putstr_fd_up("minishell: export: ", 2);
		ft_putstr_fd_up(cmd, 2);
		ft_putstr_fd_up(": not a valid identifier\n", 2);
		free(key);
		return;
	}
	if (!is_exist(*env, key))
		add_env_export(env, key, NULL);
	free(key);
}

void ft_export(t_cmd **cmd_ptr, t_env **envp, t_shell *shell)
{
	t_cmd *cmd;
	int i;
	char *assignment;
	char *append;

	i = 1;
	cmd = *cmd_ptr;
	if (cmd->cmd[0] && cmd->cmd[1] == NULL) 
	{
		if (!cmd->cmd[1])
			sort_and_display_env(envp);
		shell->exit_status = 0;
		return ;
	}
	while (cmd->cmd[i])
	{
		assignment = ft_strchr(cmd->cmd[i], '=');
		append = ft_strstr(cmd->cmd[i], "+=");
		if (append)
			handel_append(envp, cmd->cmd[i]);
		else if (assignment)
			handle_assigmnet(envp, cmd->cmd[i]);
		else
			handle_export(envp, cmd->cmd[i]);
		i++;
	}
	shell->exit_status = 0;
}