/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:50:12 by maskour           #+#    #+#             */
/*   Updated: 2025/05/22 13:14:03 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
//this to check if the key is valid
static int is_valid_key(char *key)
{
    if(!key || !*key)
        return (0);
        // the first char is letter or _
    if (!ft_isalpha_up(*key) && *key != '_')
        return 0;
    int i = 0;
    // letter deget _
    while (key[++i])
    {
        if (!ft_isalnum(key[i]) && key[i] != '_' )
            return 0;
    }
    return (1);
}
// this to separ between the key and the value
static void extra_key_value(char *input, char **key, char **value)
{
    *key = NULL;
    *value = NULL;
    char *equal = ft_strchr(input, '=');// the equal have ="path"
    char *plus = NULL;
    if (!input)
        return;
    if (equal)
    {
        if (equal > input && *(equal - 1) == '+')
        {
            plus = equal - 1;
            *plus = '\0';
        }
        else
            *equal = '\0';
        *key = input;
        *value = equal + 1;
    }
    //no value
    else
        *key = input;
}
static void add_env_export(t_env *env, char *key, char *value)
{
    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node) 
        return;

    char *env_entry;
    if (value) {
        char *tmp = ft_strjoin(key, "=");
        env_entry = ft_strjoin(tmp, value);
        free(tmp);
    } else {
        env_entry = ft_strjoin(key, "=");
    }

    if (!env_entry) 
    {
        free(new_node);
        return;
    }
    new_node->data_env = env_entry;
    new_node->next = NULL;

    if (!env) {
        env = new_node;
    } else {
        t_env *current = env;
        while (current->next)
            current = current->next;
        current->next = new_node;
    }
}

// i use babel sort+
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
       while (j < lsit_size - i - 1)
       {
            
            if (ft_strcmp(env->data_env, env->next->data_env) > 0)
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
static void sort_and_display_env(t_env *envp)
{
    t_env *tmp;

    // sort the linked list
    tmp = sort_env(envp);
    // print sorted linked list
    print_sort_env(tmp);
}
//handele this appand 
static void append_value(t_env *env, char *key, char *value)
{
    t_env *current = env;
    char *new_val;
    char *tmp;
    char *final_val;
    int key_len = ft_strlen(key);
    if (!key || !value)
        return ;
    while (current)
    {
        if (!ft_strncmp(current->data_env , key, key_len ) && current->data_env[key_len] == '=')
        {
            tmp = ft_strchr(current->data_env, '=') + 1;
            new_val = ft_strjoin(tmp, value);
            free(current->data_env);
            current->data_env = ft_strjoin(key,"=");
            final_val = ft_strjoin(current->data_env, new_val);
            free(current->data_env);
            current->data_env = final_val;
            free(new_val);
            return ;
        }
        current = current->next;
    }
    add_env_export(env,key, value);
}

// this to handel if there is join i the same key 
static void handel_append(t_env *env, char *cmd)
{
    char *key = NULL; 
    char *value = NULL;
    extra_key_value(cmd, &key, &value);
    printf("KEY:%s\n",key);
    printf("value:%s\n", value);
     ///is valide key 
    if (!is_valid_key(key))
    {
        ft_putstr_fd_up("minishell: export: ",2);
        ft_putstr_fd_up(cmd,2);
        perror(": not a valid identifier1\n");
        return ;
    }
    append_value(env, key, value);
}
static void update_or_add_env(t_env **env, char *key, char *value)
{
    t_env *current = *env;
    int key_len;
    
    if (!key)
        return;
    
    key_len = ft_strlen(key);
    while (current) {
        if (!ft_strncmp(current->data_env, key, key_len) && current->data_env[key_len] == '=') {
            char *tmp = ft_strjoin(key, "=");
            char *add_equal = ft_strjoin(tmp, value);
            free(tmp);
            free(current->data_env);
            current->data_env = add_equal;
            return;
        }
        current = current->next;
    }
    // If we got here, the key wasn't found - add it
    add_env_export(*env, key, value);
}
// this to handle just the value after the key
static void handle_assigmnet(t_env *env, char *cmd)
{
    //is valid
    char *key = NULL;
    char *value = NULL;
    extra_key_value(cmd, &key, &value);
    if (!is_valid_key(key))
    {
        ft_putstr_fd_up("minishell: export: ",2);
        ft_putstr_fd_up(cmd,2);
        perror(": not a valid identifier2\n");
        return ;
    }
    update_or_add_env(&env, key, value);
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
// static void mark_as_ecport(t_env *env, char key)
// {
//     t_env *current = env;
//     int key_len = ft_strlen(key);
//     while (current)
//     {
//         if (!ft_strcmp(current->data_env, key) && current->data_env[key_len] == '=')
            
//         return ;
//     }
// }
// this just for test
static void mark_as_exported(t_env *env, char *key)
{
    t_env *current = env;
    size_t key_len = ft_strlen(key);
    
    while (current)
    {
        // Check if this is the variable we're looking for
        if (ft_strncmp(current->data_env, key, key_len) == 0 && 
            current->data_env[key_len] == '=')
        {
            // Check if it's already marked as exported
            if (ft_strncmp(current->data_env, "export ", 7) != 0)
            {
                // Create new entry with "export " prefix
                char *new_entry = ft_strjoin("export ", current->data_env);
                if (!new_entry)
                    return;  // Memory allocation failed
                
                free(current->data_env);
                current->data_env = new_entry;
            }
            return;
        }
        current = current->next;
    }
    
    // If we get here, the variable wasn't found - add it as a new exported variable
    char *new_entry = ft_strjoin("export ", key);
    if (!new_entry)
        return;
    
    char *full_entry = ft_strjoin(new_entry, "=");
    free(new_entry);
    if (!full_entry)
        return;
    
    add_env_export(env, full_entry, NULL);
    free(full_entry);
}

static void handle_export(t_env *env, char *cmd)
{
    //is avalid
    char *empty = "";
    char *key = NULL;
    extra_key_value(cmd, &key,&empty);
 
    if (!is_valid_key(key))
    {
        ft_putstr_fd_up("minishell: export: ",2);
        ft_putstr_fd_up(cmd,2);
        perror(": not a valid identifier3\n");
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
void ft_export(t_cmd **cmd_ptr, t_env *envp)
{
    t_cmd *cmd;
    int i;
    char *assignment ;
    char *append;

    i = 1;
    cmd =*cmd_ptr;
    if (cmd->cmd[0] && cmd->cmd[1] == NULL)
    {
        if (!cmd->cmd[1])
            sort_and_display_env(envp);//sort the linked list and desplay it  //0for sort and desplay
        return ;//return the env;
    }
    while (cmd->cmd[i])
    {
        assignment = ft_strchr(cmd->cmd[i],'=');
        append = ft_strstr(cmd->cmd[i],"+=");
        if (append)
            handel_append(envp,cmd->cmd[i]);
        else if (assignment)
            handle_assigmnet(envp,cmd->cmd[i]);
        else // ths for no value
            handle_export(envp,cmd->cmd[i]);
        i++;
    }
}
