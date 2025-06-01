/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:50:12 by maskour           #+#    #+#             */
/*   Updated: 2025/06/01 15:20:57 by maskour          ###   ########.fr       */
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
            printf("=\"%s\"\n", eq + 1); // always print quotes (bash does)
        } else {
            printf("declare -x %s\n", current->data_env);
        }
        current = current->next;
    }
}
static int size_list(t_env *env) {
    int i = 0;
    while (env) {
        env = env->next;
        i++;
    }
    return (i);
}
// Sort env list (bubble sort, pointer swap)
static t_env *sort_env(t_env *env) {
    if (!env || !env->next)
        return env;
    t_env *head = env;
    int swapped;
    int list_size = size_list(env);
    for (int i = 0; i < list_size - 1; i++) {
        t_env *prev = NULL;
        t_env *curr = head;
        t_env *next = head->next;
        swapped = 0;
        for (int j = 0; j < list_size - i - 1; j++) {
            if (ft_strcmp(curr->data_env, next->data_env) > 0) {
                // Swap pointers, not just data_env
                if (prev)
                    prev->next = next;
                else
                    head = next;
                curr->next = next->next;
                next->next = curr;
                swapped = 1;
                prev = next;
                next = curr->next;
            } else {
                prev = curr;
                curr = next;
                next = next->next;
            }
        }
        if (!swapped)
            break;
    }
    return head;
}
static void sort_and_display_env(t_env *envp) 
{
    t_env *tmp = sort_env(envp);
    print_sort_env(tmp);
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
        if (!ft_strncmp(current->data_env, key, key_len) && current->data_env[key_len] == '=')
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
    char *equal = ft_strchr(input, '=');
    if (equal) {
        if (equal > input && *(equal - 1) == '+') {
            *(equal - 1) = '\0';
        } else {
            *equal = '\0';
        }
        *key = input;
        *value = equal + 1;
    } else {
        *key = input;
    }
}
static void add_env_export(t_env *env, char *key, char *value) {
    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node) return;
    char *env_entry;
    if (value) {
        char *tmp = ft_strjoin(key, "=");
        if (!tmp) { free(new_node); return; }
        env_entry = ft_strjoin(tmp, value);
        free(tmp);
        if (!env_entry) { free(new_node); return; }
    } else {
        env_entry = ft_strjoin(key, "=");
        if (!env_entry) { free(new_node); return; }
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

// Update or add env (memory safe)
static void update_or_add_env(t_env *env, char *key, char *value) {
    t_env *current = env;
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
static void append_value(t_env *env, char *key, char *value) {
    t_env *current = env;
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

// Handle append (export KEY+=value)
static void handel_append(t_env *env, char *cmd) {
    char *key = NULL;
    char *value = NULL;
    extra_key_value(cmd, &key, &value);
    if (!is_valid_key(key)) {
        ft_putstr_fd_up("minishell: export: ", 2);
        ft_putstr_fd_up(cmd, 2);
        perror(": not a valid identifier1\n");
        free(key); free(value);
        return;
    }
    append_value(env, key, value);
    free(key); free(value);
}

// Handle assignment (export KEY=value)
static void handle_assigmnet(t_env *env, char *cmd) {
    char *key = NULL;
    char *value = NULL;
    extra_key_value(cmd, &key, &value);
    if (!is_valid_key(key)) {
        ft_putstr_fd_up("minishell: export: ", 2);
        ft_putstr_fd_up(cmd, 2);
        perror(": not a valid identifier2\n");
        free(key); free(value);
        return;
    }
    update_or_add_env(env, key, value);
    free(key); free(value);
}

// Handle export KEY (no value, just mark as exported if not present)
static void handle_export(t_env *env, char *cmd) {
    char *empty = "";
    char *key = NULL;
    extra_key_value(cmd, &key, &empty);
    if (!is_valid_key(key)) {
        ft_putstr_fd_up("minishell: export: ", 2);
        ft_putstr_fd_up(cmd, 2);
        perror(": not a valid identifier3\n");
        free(key);
        return;
    }
    if (!is_exist(env, key))
        add_env_export(env, key, empty);
    free(key);
}

// Export global function
void ft_export(t_cmd **cmd_ptr, t_env *envp) 
{
    t_cmd *cmd;
    int i;
    char *assignment;
    char *append;

    i = 1;
    cmd = *cmd_ptr;
    if (cmd->cmd[0] && cmd->cmd[1] == NULL) {
        if (!cmd->cmd[1])
            sort_and_display_env(envp);
        return;
    }
    while (cmd->cmd[i]) {
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
}
// Check if the key is valid
// static int is_valid_key(char *key) 
// {
//     if (!key || !*key)
//         return (0);
//     if (!ft_isalpha_up(*key) && *key != '_')
//         return 0;
//     int i = 0;
//     while (key[++i]) {
//         if (!ft_isalnum(key[i]) && key[i] != '_')
//             return 0;
//     }
//     return (1);
// }

// // Separate key and value (handle += as well)
// static void extra_key_value(char *input, char **key, char **value) {
//     *key = NULL;
//     *value = NULL;
//     if (!input)
//         return;
//     char *equal = ft_strchr(input, '=');
//     if (equal) {
//         if (equal > input && *(equal - 1) == '+') {
//             *(equal - 1) = '\0';
//         } else {
//             *equal = '\0';
//         }
//         *key = input;
//         *value = equal + 1;
//     } else {
//         *key = input;
//     }
// }

// // Add variable to env list (memory/pointer safe)
// static void add_env_export(t_env *env, char *key, char *value) {
//     t_env *new_node = malloc(sizeof(t_env));
//     if (!new_node)
//         return;
//     char *env_entry;
//     if (value) {
//         char *tmp = ft_strjoin(key, "=");
//         if (!tmp) {
//             free(new_node);
//             return;
//         }
//         env_entry = ft_strjoin(tmp, value);
//         free(tmp);
//         if (!env_entry) {
//             free(new_node);
//             return;
//         }
//     } else {
//         env_entry = ft_strjoin(key, "=");
//         if (!env_entry) {
//             free(new_node);
//             return;
//         }
//     }
//     new_node->data_env = env_entry;
//     new_node->next = NULL;

//     if (!env) {
//         env = new_node;
//     } else {
//         t_env *current = env;
//         while (current->next)
//             current = current->next;
//         current->next = new_node;
//     }
// }

// // Count list size
// static int size_list(t_env *env) {
//     int i = 0;
//     while (env) {
//         env = env->next;
//         i++;
//     }
//     return (i);
// }

// // Sort env list (bubble sort, pointer swap)
// static t_env *sort_env(t_env *env) {
//     if (!env || !env->next)
//         return env;
//     t_env *head = env;
//     int swapped;
//     int list_size = size_list(env);
//     for (int i = 0; i < list_size - 1; i++) {
//         t_env *prev = NULL;
//         t_env *curr = head;
//         t_env *next = head->next;
//         swapped = 0;
//         for (int j = 0; j < list_size - i - 1; j++) {
//             if (ft_strcmp(curr->data_env, next->data_env) > 0) {
//                 // Swap pointers, not just data_env
//                 if (prev)
//                     prev->next = next;
//                 else
//                     head = next;
//                 curr->next = next->next;
//                 next->next = curr;
//                 swapped = 1;
//                 prev = next;
//                 next = curr->next;
//             } else {
//                 prev = curr;
//                 curr = next;
//                 next = next->next;
//             }
//         }
//         if (!swapped)
//             break;
//     }
//     return head;
// }

// // Print env in bash format (declare -x ...)
// static void print_sort_env(t_env *env) {
//     t_env *current = env;
//     while (current) {
//         char *eq = ft_strchr(current->data_env, '=');
//         if (eq) {
//             int key_len = eq - current->data_env;
//             printf("declare -x ");
//             fwrite(current->data_env, 1, key_len, stdout);
//             printf("=\"%s\"\n", eq + 1); // always print quotes (bash does)
//         } else {
//             printf("declare -x %s\n", current->data_env);
//         }
//         current = current->next;
//     }
// }

// // Sort and display env
// static void sort_and_display_env(t_env *envp) {
//     t_env *tmp = sort_env(envp);
//     print_sort_env(tmp);
// }

// // Append value to an existing key, or add if not found (memory safe)
// static void append_value(t_env *env, char *key, char *value) {
//     t_env *current = env;
//     int key_len = ft_strlen(key);
//     if (!key || !value)
//         return;
//     while (current) {
//         if (!ft_strncmp(current->data_env, key, key_len) && current->data_env[key_len] == '=') {
//             char *old_val = ft_strchr(current->data_env, '=') + 1;
//             char *new_val = ft_strjoin(old_val, value);
//             if (!new_val)
//                 return;
//             char *tmp = ft_strjoin(key, "=");
//             if (!tmp) {
//                 free(new_val);
//                 return;
//             }
//             char *final_val = ft_strjoin(tmp, new_val);
//             if (!final_val) {
//                 free(tmp);
//                 free(new_val);
//                 return;
//             }
//             free(current->data_env);
//             current->data_env = final_val;
//             free(tmp);
//             free(new_val);
//             return;
//         }
//         current = current->next;
//     }
//     add_env_export(env, key, value);
// }

// // Handle append (export KEY+=value)
// static void handel_append(t_env *env, char *cmd) {
//     char *key = NULL;
//     char *value = NULL;
//     extra_key_value(cmd, &key, &value);
//     if (!is_valid_key(key)) {
//         ft_putstr_fd_up("minishell: export: ", 2);
//         ft_putstr_fd_up(cmd, 2);
//         perror(": not a valid identifier1\n");
//         return;
//     }
//     append_value(env, key, value);
// }

// // Update or add env (memory safe)
// static void update_or_add_env(t_env *env, char *key, char *value) {
//     t_env *current = env;
//     int key_len;
//     if (!key)
//         return;
//     key_len = ft_strlen(key);
//     while (current) {
//         if (!ft_strncmp(current->data_env, key, key_len) && current->data_env[key_len] == '=') {
//             char *tmp = ft_strjoin(key, "=");
//             if (!tmp)
//                 return;
//             char *add_equal = ft_strjoin(tmp, value);
//             free(tmp);
//             if (!add_equal)
//                 return;
//             free(current->data_env);
//             current->data_env = add_equal;
//             return;
//         }
//         current = current->next;
//     }
//     add_env_export(env, key, value);
// }

// // Handle assignment (export KEY=value)
// static void handle_assigmnet(t_env *env, char *cmd) {
//     char *key = NULL;
//     char *value = NULL;
//     extra_key_value(cmd, &key, &value);
//     if (!is_valid_key(key)) {
//         ft_putstr_fd_up("minishell: export: ", 2);
//         ft_putstr_fd_up(cmd, 2);
//         perror(": not a valid identifier2\n");
//         return;
//     }
//     update_or_add_env(env, key, value);
// }

// // Check if key exists in env
// static int is_exist(t_env *env, char *key) {
//     if (!key)
//         return (0);
//     t_env *current = env;
//     int key_len = ft_strlen(key);
//     while (current) {
//         if (!ft_strncmp(current->data_env, key, key_len) && current->data_env[key_len] == '=')
//             return (1);
//         current = current->next;
//     }
//     return (0);
// }

// // Handle export KEY (no value, just mark as exported if not present)
// static void handle_export(t_env *env, char *cmd) {
//     char *empty = "";
//     char *key = NULL;
//     extra_key_value(cmd, &key, &empty);
//     if (!is_valid_key(key)) {
//         ft_putstr_fd_up("minishell: export: ", 2);
//         ft_putstr_fd_up(cmd, 2);
//         perror(": not a valid identifier3\n");
//         return;
//     }
//     // If it's not exist, add it with empty value
//     if (!is_exist(env, key))
//         add_env_export(env, key, empty);
//     // If it exists, do nothing (already exported)
// }

// // Export global function
// void ft_export(t_cmd **cmd_ptr, t_env *envp) 
// {
//     t_cmd *cmd;
//     int i;
//     char *assignment;
//     char *append;

//     i = 1;
//     cmd = *cmd_ptr;
//     if (cmd->cmd[0] && cmd->cmd[1] == NULL) {
//         if (!cmd->cmd[1])
//             sort_and_display_env(envp);
//         return;
//     }
//     while (cmd->cmd[i]) {
//         assignment = ft_strchr(cmd->cmd[i], '=');
//         append = ft_strstr(cmd->cmd[i], "+=");
//         if (append)
//             handel_append(envp, cmd->cmd[i]);
//         else if (assignment)
//             handle_assigmnet(envp, cmd->cmd[i]);
//         else
//             handle_export(envp, cmd->cmd[i]);
//         i++;
//     }
// }
