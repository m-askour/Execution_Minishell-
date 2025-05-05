/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:50:12 by maskour           #+#    #+#             */
/*   Updated: 2025/05/05 15:50:00 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
//this to check if the key is valid
static int is_valid_key(char *key)
{
    if(!key || !*key)
        return (0);
        // the first char is letter or _
    if (!ft_isalpha(*key) && *key != "_")
        return 0;
    int i = 0;
    // letter deget _
    while (key[++i])
    {
        if (!ft_isalnum(key[i]) && key[i] != "_")
            return 0;
        i++;
    }
    return (1);
}
// this to separ between the key and the value
static void extra_key_value(char *input, char **key, char **value)
{
    *key = NULL;
    *value = NULL;
    char *equal = ft_strchr(input, "=");// the equal have ="path"
    if (!input)
        return;
    if (equal)
    {
        *equal = "\0";
        *key = input;
        *value = equal + 1;
    }
    //no value
    else
        *key = input;
}
static void add_env_export(t_env **env, char *key , char *value)
{
    t_env *current = env;
    t_env *new_node = malloc(sizeof(t_env));
    if (new_node)
    {
        free(new_node);
        return ;
    }
    char *env_entry;
    if (value) // exist value
    {
        env_entry = ft_strjoin(key, "=");
        env_entry = ft_strjoin(env_entry, value);
    }
    else
        env_entry = ft_strjoin(key, "=");
    //filed strjoin
    if (!env_entry)
    {
        free(new_node);
        return;
    }
    new_node->data_env = env_entry;
    new_node->next = NULL;
    if (!*env)
        *env = new_node;
    
    else
    {
        while(current)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}
// i use babel sort
static int size_list(t_env *env)
{

    int i = 0;
    while (env)
    {
        env = env->next;
        i++;
    }
    return (i);
}
// sort the linked list use babel sort
static t_env *sort_env(t_env *env)
{
    if (!env || !env->next)
        return (env);
    t_env *head;
    int swaaped;
    char *tmp;
    head = env;
    swaaped = 0;
    int lsit_size = size_list(env);
    int i = 0 ;
    int j;
    while (i < lsit_size - 1)
    {
       // declare -x key = value
       env = head;
       swaaped = 0;// this to see is there is wha sort or no 
       j = 0;
       while (i < lsit_size - i - 1)
       {
            
            if (ft_strcmd(env->data_env, env->next->data_env) > 0)
            {
                tmp = env->data_env;
                env->data_env = env->next->data_env;
                env->next->data_env = tmp;
                swaaped = 1;
            }
            env = env->next;
            j++;
       }
       if (!swaaped)
        break;
       i++;
    }
    return (head);
}
//this to desplay what in env sortted
static void print_sort_env(t_env *env)
{
    t_env *current = env;
    while (current)
    {
        printf("declare -x %s\n", current->data_env);  /* code */
        current = current->next;
    }
}
//this for sort the env and desplayet in 1;
static void sort_and_display_env(t_env *envp, int i)
{
    t_env *tmp;
    if (i = 0)
    {
        // sort the linked list
        tmp = sort_env(envp);

        // print sorted linked list
        print_sort_env(tmp);
        
    }
    else if (i = 1)
        sort_env(envp);//sort the linked list
        
}
static void append_value(t_env *env, char *key, char *value)
{
    t_env *current = env;
    if (!key)
        return;
    while (current)
    {
        if (!ft_strcmp(current->data_env , key ))
            current->data_env = ft_strjoin(current->data_env, value);
        add_env_export(env,key, value);
        current = current->next;  
    }
}

// this to handel if there is join i the same key 
static void handel_append(t_env *env, char *key, char *value, char *cmd)
{
    // is valide key 
    if (!is_valid_key)
    {
        ft_putstr_fd_up("minishell: export: ",2);
        ft_putstr_fd_up(cmd,2);
        perror(": not a valid identifier\n");
        return ;
    }
    append_value(env, key, value);
}
static void update_or_add_env(t_env *env, char *key, char value)
{
    t_env *current = env;
    if (!key)
        return ;
    char *add_equal;
    while(current)
    {
        if (!ft_strcmp(current->data_env , key ))
        {
                free(current->data_env);
                
                add_equal= ft_strjoin(key, "=");
                current->data_env = ft_strjoin(add_equal, value);
    
        }
        else
            add_env_export(env,key, value);
        current = current->next;
    }
}
// this to handle just the value after the key
static void handle_assigmnet(t_env *env, char *key, char *value, char *cmd)
{
    //is valid
    if (!is_valid_key)
    {
        ft_putstr_fd_up("minishell: export: ",2);
        ft_putstr_fd_up(cmd,2);
        perror(": not a valid identifier\n");
        return ;
    }
    update_or_add_env(env, key, value);
}
// handel the export general 
static int is_exist(t_env *env, char *key)
{   
    if (!key)
        return (0);
    t_env *current = env;
    while (current)
    {
        if(!ft_strcmp(current->data_env, key))
            return (1);
        current = current->next;
    }   
    return(0);
}
static void mark_as_ecport(t_env *env, char key)
{
    t_env *current = env;
    int key_len = ft_strlen(key);
    while (current)
    {
        if (!ft_strcmp(current->data_env, key) && current->data_env[key_len] == '=')
            
        return ;
    }
}
static void handle_export(t_env *env, char *key, char *value, char *cmd)
{
    //is avalid
    char *empty = "";
    if (!is_valid_key(key))
    {
        ft_putstr_fd_up("minishell: export: ",2);
        ft_putstr_fd_up(cmd,2);
        perror(": not a valid identifier\n");
        return ;
    }
    //it's exist ke in env
    if (is_exist(env,key))
        mark_as_exported(env, key);
    //if it's not exist
    else
        add_env_export(env,key, empty);
}
// this is the global function 
void ft_export(t_cmd **cmd, t_env *envp)
{
    char *key;
    char *value;
    if (cmd[0]->cmd[0])
    {
        if (!cmd[0]->cmd[1])
            sort_and_display_env(envp,0);//sort the linked list and desplay it  //0for sort and desplay
        return ;//return the env;
    }
    int i = 0;
    while (cmd[0]->cmd[i])
    {
        extra_key_value(cmd[0]->cmd[i], &key, &value);
        if (!ft_strcmp(cmd[0]->cmd[i],"=")){
            if (!ft_strcmp(cmd[0]->cmd[i],"+="))
                handel_append(envp, key, value, cmd[0]->cmd[i]);
            else
                handle_assigmnet(envp, key, value, cmd[0]->cmd[i]);
        }
        // ths for no value
        else
            handel_export(envp, key, value, cmd[0]->cmd[i]);
        i++;
    }
    sort_and_display_env(envp,1);// 1 just for sort not desplay 
}