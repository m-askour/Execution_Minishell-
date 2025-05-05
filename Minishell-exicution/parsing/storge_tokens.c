/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   storge_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:49:49 by ahari             #+#    #+#             */
/*   Updated: 2025/04/24 16:06:56 by ahari            ###   ########.fr       */
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
    return (cmd);
}

t_cmd **parse_commands(t_token *tokens)
{
    int     cmd_count;
    int     i;
    t_cmd   **cmd_list;

    i = 0;
    cmd_count = count_commands(tokens);
    cmd_list = malloc(sizeof(t_cmd *) * (cmd_count + 1));
    if (!cmd_list)
        return (NULL);
    while (i < cmd_count)
    {
        cmd_list[i] = parse_single_command(&tokens);
        if (!cmd_list[i])
            return (free_cmd_list(cmd_list, i), NULL);
        if (tokens && tokens->type == TOKEN_PIPE && tokens->next)
            tokens = tokens->next;
        else if (tokens && tokens->type == TOKEN_PIPE && !tokens->next)
            return (free_cmd_list(cmd_list, i),
                ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2, 0), NULL);
        i++;
    }
    cmd_list[cmd_count] = NULL;
    i = 0;
    int count = 0;
    while (cmd_list[i])
    {
        if(cmd_list[i]->files->type == TOKEN_HEREDOC)
            count++;
        i++;
    }
    if(count > 24)
    {
        ft_putstr_fd("minishell: maximum here-document count exceeded\n", 2, 0);
        return (free_cmd_list(cmd_list, i), NULL);
    }
    return cmd_list;
}
