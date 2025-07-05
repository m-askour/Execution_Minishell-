/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:50:12 by maskour           #+#    #+#             */
/*   Updated: 2025/07/05 18:57:36 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void print_sort_env(t_env *env) {
    t_env *current = env;
    while (current) {
        char *eq = ft_strchr(current->data_env, '=');
        if (eq) {
            int key_len = eq - current->data_env;
            printf("declare -x ");
            fwrite(current->data_env, 1, key_len, stdout);
            printf("=\"%s\"\n", eq + 1);
        } else {
            // Export-only variable: just the key
            printf("declare -x %s\n", current->data_env);
        }
        current = current->next;
    }
}

static t_env *sort_env(t_env *env) {
    if (!env || !env->next) 
        return env;

    int swapped;
    t_env *ptr1;
    t_env *lptr = NULL;
    swapped = 1;
    while (swapped) 
    {
        swapped = 0;
        ptr1 = env;

        while (ptr1->next != lptr) {
            if (ft_strcmp(ptr1->data_env, ptr1->next->data_env) > 0) {
                // Swap data_env (simpler than pointer swaps)
                char *tmp = ptr1->data_env;
                ptr1->data_env = ptr1->next->data_env;
                ptr1->next->data_env = tmp;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }

    return env;
}

static void sort_and_display_env(t_env **envp) 
{
    *envp = sort_env(*envp); // Sort and update the list
    print_sort_env(*envp);   // Print the sorted list
}
static int is_valid_key(char *key) 
{
    if (!key || !*key)
        return (0);
    if (!ft_isalpha_up(*key) && *key != '_')
        return 0;
    int i = 0;
    while (key[++i]) {
        if (!ft_isalnum(key[i]) && key[i] != '_')
            return 0;
    }
    return (1);
}
static int is_exist(t_env *env, char *key) {
    if (!key)
        return (0);
    t_env *current = env;
    int key_len = ft_strlen(key);
    while (current) {
        // Accept match either if it's export-only or has '='
        if (!ft_strncmp(current->data_env, key, key_len) &&
            (current->data_env[key_len] == '=' || current->data_env[key_len] == '\0'))
            return (1);
        current = current->next;
    }
    return (0);
}
static void extra_key_value(char *input, char **key, char **value) {
    *key = NULL;
    *value = NULL;
    if (!input)
        return;

    char *input_copy = ft_strdup(input); // Use ft_strdup (safer)
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
    free(input_copy); // Free the temporary copy
}
static void add_env_export(t_env **env, char *key, char *value) {
    t_env *current = *env;
    int key_len = strlen(key);

    // Check if key exists (either as export-only or with value)
    while (current) {
        // Match export-only or with value
        if (!strncmp(current->data_env, key, key_len) &&
            (current->data_env[key_len] == '=' || current->data_env[key_len] == '\0')) 
        {
            // Update existing node to new value
            free(current->data_env);
            if (value) {
                char *tmp = ft_strjoin(key, "=");
                if (!tmp) return;
                current->data_env = ft_strjoin(tmp, value);
                free(tmp);
            } else {
                current->data_env = strdup(key); // export-only
            }
            return; // Important! Do not add a new node
        }
        current = current->next;
    }

    // Doesn't exist, add new node
    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node) return;
    if (value) {
        char *tmp = ft_strjoin(key, "=");
        if (!tmp) { free(new_node); return; }
        new_node->data_env = ft_strjoin(tmp, value);
        if (!new_node)
        {
            free(tmp);
            free(new_node);
            return ;
        }
        free(tmp);
    } else {
        new_node->data_env = strdup(key); // export-only
        if (!new_node->data_env){
            free(new_node);
            return ;
        }
    }
    new_node->next = NULL;
    if (!*env) {
        *env = new_node;
    } else {
        t_env *last = *env;
        while (last->next)
            last = last->next;
        last->next = new_node;
    }
}
static void update_or_add_env(t_env **env, char *key, char *value) 
{
    t_env *current = *env;
    int key_len;
    if (!key) return;
    key_len = ft_strlen(key);
    while (current) {
        if (!ft_strncmp(current->data_env, key, key_len) && current->data_env[key_len] == '=') {
            char *tmp = ft_strjoin(key, "=");
            if (!tmp) return;
            char *add_equal = ft_strjoin(tmp, value);
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

// Append value to an existing key, or add if not found (memory safe)
static void append_value(t_env **env, char *key, char *value) 
{
    t_env *current = *env;
    int key_len = ft_strlen(key);
    if (!key || !value)
        return;
    while (current) {
        if (!ft_strncmp(current->data_env, key, key_len) && current->data_env[key_len] == '=') {
            char *old_val = ft_strchr(current->data_env, '=') + 1;
            char *new_val = ft_strjoin(old_val, value);
            if (!new_val)
                return;
            char *tmp = ft_strjoin(key, "=");
            if (!tmp) {
                free(new_val);
                return;
            }
            char *final_val = ft_strjoin(tmp, new_val);
            if (!final_val) {
                free(tmp);
                free(new_val);
                return;
            }
            free(current->data_env);
            current->data_env = final_val;
            free(tmp);
            free(new_val);
            return;
        }
        current = current->next;
    }
    add_env_export(env, key, value);
}


static int  handel_append(t_env **env, char *cmd) {
    char *key = NULL;
    char *value = NULL;
    extra_key_value(cmd, &key, &value);
    if (!is_valid_key(key)) {
        ft_putstr_fd_up("minishell: export: ", 2);
        ft_putstr_fd_up(cmd, 2);
        perror(": not a valid identifier1\n");
        free(key); free(value);
        return (1);
    }
    append_value(env, key, value);
    free(key); free(value);
    return (0);
}

static int  handle_assigmnet(t_env **env, char *cmd) {
    char *key = NULL;
    char *value = NULL;
    extra_key_value(cmd, &key, &value);
    if (!is_valid_key(key)) {
        ft_putstr_fd_up("minishell: export: ", 2);
        ft_putstr_fd_up(cmd, 2);
        ft_putstr_fd_up(": not a valid identifier2\n",2);
        free(key); free(value);
        return (1);
    }
    update_or_add_env(env, key, value);
    free(key); free(value);
    return (0);
}

static int handle_export(t_env **env, char *cmd) 
{
    char *empty = "";
    char *key = NULL;
    extra_key_value(cmd, &key, &empty);
    if (!is_valid_key(key)) {
        ft_putstr_fd_up("minishell: export: ", 2);
        ft_putstr_fd_up(cmd, 2);
        ft_putstr_fd_up(": not a valid identifier\n", 2);
        free(key);
        return (1);
    }
    // If the key exists, ensure it's exported (even if it had no value before)
    if (!is_exist(*env, key))
        add_env_export(env, key, NULL);
    free(key);
    return (0);
}

void ft_export(t_cmd **cmd_ptr, t_env **envp, t_shell *shell) // NOTE: envp is a pointer to pointer!
{
    t_cmd *cmd;
    int i;
    char *assignment;
    char *append;
    int had_error = 0;
    i = 1;
    cmd = *cmd_ptr;
    if (cmd->cmd[0] && cmd->cmd[1] == NULL) 
    {
        if (!cmd->cmd[1])
            sort_and_display_env(envp);
        shell->exit_status = 0;
        return;
    }
    while (cmd->cmd[i]) {
        assignment = ft_strchr(cmd->cmd[i], '=');
        append = ft_strstr(cmd->cmd[i], "+=");
        if (append)
        {
            if (handel_append(envp, cmd->cmd[i]) == 1)
                had_error = 1;
        }
        else if (assignment)
        {    if (handle_assigmnet(envp, cmd->cmd[i]) == 1)
                had_error = 1;}
        else
{            if (handle_export(envp, cmd->cmd[i]) == 1)
                had_error = 1;}
        i++;
    }
    if (had_error)
    shell->exit_status = 1;
    else
    shell->exit_status = 0;

}
