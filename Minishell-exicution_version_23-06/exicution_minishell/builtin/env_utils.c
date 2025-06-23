/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:39:24 by maskour           #+#    #+#             */
/*   Updated: 2025/06/23 21:57:50 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	remove_env_key (t_env **env, const char *key) 
{
    t_env	*current; 
	t_env	*prev;
    int		key_len;
	
	current = *env;
	prev = NULL;
	key_len = strlen(key);
    while (current) 
	{
        if (!strncmp(current->data_env, key, key_len) &&
            (current->data_env[key_len] == '=' || current->data_env[key_len] == '\0')) 
        {
            if (prev)
                prev->next = current->next;
            else
                *env = current->next;

            free(current->data_env);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

t_env	*new_env(char *data_env)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->data_env = ft_strdup(data_env);
	if (!node->data_env)
	{
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

static t_env	*default_env(t_env *env_list)
{
	t_env	*tail = NULL;
	t_env	*new = NULL;
	char	cwd[1024];
	char	pwd_entry[1100];

	tail = NULL;
	new = NULL;
	if (getcwd(cwd, sizeof(cwd))) 
	{
	   ft_strcpy(pwd_entry, "PWD=");
	   ft_strcat(pwd_entry, cwd);
		new = new_env(pwd_entry);
		if (!new)
			return NULL;
		env_list = tail = new;
	}
	new = new_env("SHLVL=1");
	if (!new) 
	{
		while (env_list) {
			t_env *tmp = env_list;
			env_list = env_list->next;
			free(tmp->data_env);
			free(tmp);
		}
		return NULL;
	}
	tail->next = new;
	tail = new;
	new = new_env("_=/usr/bin/env");
	if (!new) {
		while (env_list) {
			t_env *tmp = env_list;
			env_list = env_list->next;
			free(tmp->data_env);
			free(tmp);
		}
		return NULL;
	}
	tail->next = new;
	tail = new;
	new = new_env("PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
	if (!new) {
		while (env_list) {
			t_env *tmp = env_list;
			env_list = env_list->next;
			free(tmp->data_env);
			free(tmp);
		}
		return NULL;
	}
	tail->next = new;
	return env_list;
}

void	free_env_list(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		env_list = current->next;
		free(current->data_env);
		free(current);
		current = env_list;
	}
	
}

char	*search_env(t_env *env, const char *key)
{
	int len;

	len = ft_strlen(key);
	while (env)
	{
		if (!ft_strncmp(env->data_env, key,len) &&  env->data_env[len] == '=')
			return(env->data_env +len + 1);  
		env = env->next;
	}
	return (NULL);
}

void	add_env(t_env **env_list, t_env *new_node)
{
	t_env	*tmp;

	if (!*env_list)
		*env_list = new_node;
	else
	{
		tmp = *env_list;
		while(tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

t_env	*file_inv(char **env)
{
	t_env	*env_list;
	t_env	*new;
	int		i;

	env_list = NULL;
	new = NULL;
	if (!env || !env[0])
		return default_env(env_list);
	i = 0;
	while(env[i])
	{
		new = new_env(env[i]);
		if (!new)
		{
			free_env_list(env_list);
			return (NULL);
		}
		add_env(&env_list,new);
		i++;
	}
	remove_env_key(&env_list, "OLDPWD");

	return (env_list);
}
