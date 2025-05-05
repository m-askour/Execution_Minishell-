/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:39:24 by maskour           #+#    #+#             */
/*   Updated: 2025/05/03 09:35:03 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char *search_env(t_env *env, const char *key)
{
    int len = ft_strle(key);
    while (env)
    {
        if (!ft_strcmp(env->data_env, key))
            return(env->data_env + len);
        env = env->next;
    }
    return (0);
}
t_env *new_env(char *data_env)
{
    t_env *node;
    node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);

    node->data_env = ft_strdup(data_env);
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
    int i = 0;
    if (!env)
        return NULL;
    while(env[i])
    {
        add_env(&env_list,new_env(env[i]));
        i++;
    }
    return (env_list);
}