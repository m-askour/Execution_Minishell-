/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:49:49 by ahari             #+#    #+#             */
/*   Updated: 2025/05/13 12:00:00 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int count_commands(t_token *tokens)
{
    int     count;
    t_token *current;
    
    count = 1;
    current = tokens;
    while (current)
    {
        if (current->type == TOKEN_PIPE)
            count++;
        current = current->next;
    }
    return (count);
}

static int parse_arguments(t_cmd *cmd, t_token **tokens)
{
    int     arg_i;
    t_token *current;
    
    arg_i = 0;
    current = *tokens;
    while (current && current->type != TOKEN_PIPE)
    {
        if (current->type == TOKEN_WORD)
        {
            cmd->cmd[arg_i] = ft_strdup(current->value);
            if (!cmd->cmd[arg_i])
                return 0;
            arg_i++;
        }
        else if (ft_isredirect(current->type) && current->next)
            current = current->next;
        current = current->next;
    }
    cmd->cmd[arg_i] = NULL;
    *tokens = current;
    return 1;
}

static int parse_redirections(t_cmd *cmd, t_token **tokens)
{
    t_file *file;
    t_token *current = *tokens;

    while (current && current->type != TOKEN_PIPE)
    {
        if (ft_isredirect(current->type))
        {
            if (!current->next)
            {
                ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2, 0);
                return 0;
            }
            if (current->next->type != TOKEN_WORD)
            {
                ft_putstr_fd("bash: syntax error near unexpected token `", 2, 0);
                if (current->next->value)
                    ft_putstr_fd(current->next->value, 2, 0);
                else
                    ft_putstr_fd("newline", 2, 0);
                ft_putstr_fd("'\n", 2, 0);
                return 0;
            }
            file = init_mfile();
            if (!file)
                return (0);
            file->name = ft_strdup(current->next->value);
            file->type = current->type;
            if (!file->name)
                return (free(file), 0);
            cmd->files[cmd->file_count++] = *file;
            free(file);
            current = current->next;
        }
        current = current->next;
    }
    *tokens = *tokens;
    return (1);
}

static t_cmd *parse_single_command(t_token **tokens)
{
    t_cmd   *cmd;
    t_token *token_start;
    int     argc;

    token_start = *tokens;
    cmd = init_cmd();
    if (!cmd)
        return (NULL);
    argc = count_args(token_start);
    cmd->cmd = malloc(sizeof(char *) * (argc + 1));
    cmd->files = malloc(sizeof(t_file) * (argc + 1));
    if (!cmd->cmd || !cmd->files)
        return (free_cmd(cmd), NULL);
    if (!parse_arguments(cmd, tokens) || 
        !parse_redirections(cmd, &token_start))
        return (free_cmd(cmd), NULL);
    while (*tokens && (*tokens)->type != TOKEN_PIPE)
        *tokens = (*tokens)->next;
    cmd->next = NULL;  // Initialize next pointer to NULL
    return (cmd);
}

t_cmd *parse_commands(t_token *tokens)
{
    t_cmd   *cmd_head = NULL;  // Head of the command linked list
    t_cmd   *current = NULL;   // Current command node
    t_cmd   *new_cmd;          // New command to be added
    int     cmd_count;
    int     i;

    if (!tokens)
        return (NULL);
    
    cmd_count = count_commands(tokens);
    i = 0;
    while (i < cmd_count)
    {
        new_cmd = parse_single_command(&tokens);
        if (!new_cmd)
        {
            // Clean up and return NULL if parsing failed
            free_cmd_list(cmd_head);
            return (NULL);
        }
        
        // Add the new command to our linked list
        if (!cmd_head)
        {
            cmd_head = new_cmd;  // First command becomes the head
            current = cmd_head;
        }
        else
        {
            current->next = new_cmd;  // Add to the end of the list
            current = current->next;
        }
        
        // Move to the next command (skip pipe token)
        if (tokens && tokens->type == TOKEN_PIPE && tokens->next)
            tokens = tokens->next;
        else if (tokens && tokens->type == TOKEN_PIPE && !tokens->next)
        {
            ft_putstr_fd("minishell: syntax error \n", 2, 0);
            free_cmd_list(cmd_head);
            return (NULL);
        }
        i++;
    }
    
    return cmd_head;
}