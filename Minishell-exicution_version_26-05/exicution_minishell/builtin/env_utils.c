/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:39:24 by maskour           #+#    #+#             */
/*   Updated: 2025/05/26 22:15:46 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void free_env_list(t_env *env_list)
{
    t_env *current;
    current = env_list;
    while (current)
    {
        env_list = current->next;
        free(current->data_env);
        free(current);
        current = env_list;
    }
    
}
char *search_env(t_env *env, const char *key)
{
    int len = ft_strlen(key);
    while (env)
    {
        if (!ft_strncmp(env->data_env, key,len) &&  env->data_env[len] == '=')
            return(env->data_env +len + 1);  
        env = env->next;
    }
    return (NULL);
}
t_env *new_env(char *data_env)
{
    t_env *node;
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
void add_env(t_env **env_list, t_env *new_node)
{
    t_env *tmp;
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
t_env *file_inv(char **env)
{
    t_env *env_list = NULL;
    t_env *new = NULL;
    int i = 0;
    if (!env)
        return NULL;
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
    return (env_list);
}