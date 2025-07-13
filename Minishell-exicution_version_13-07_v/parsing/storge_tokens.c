/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   storge_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:49:49 by ahari             #+#    #+#             */
/*   Updated: 2025/07/13 22:19:49 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int count_commands(t_token *tokens)
{
    int count = 1;
    while (tokens)
    {
        if (tokens->type == TOKEN_PIPE)
            count++;
        tokens = tokens->next;
    }
    return count;
}
char *remove_char(const char *str, char to_remove)
{
    int i = 0, j = 0;
    char *result;

    if (!str)
        return NULL;

    // Allocate memory for the resulting string (same size as the original string)
    result = malloc(strlen(str) + 1);
    if (!result)
        return NULL;

    // Iterate through the original string
    while (str[i])
    {
        // Copy characters that are not the one to remove
        if (str[i] != to_remove)
        {
            result[j++] = str[i];
        }
        i++;
    }

    // Null-terminate the resulting string
    result[j] = '\0';

    return result;
}
static int parse_arguments(t_cmd *cmd, t_token *tokens)
{
    int arg_i = 0;
    t_token *current = tokens;
    if (!cmd->cmd)
        return 0;
    while (current && current->type != TOKEN_PIPE)
    {
        if (current->type == TOKEN_WORD && 
            ft_strcmp(current->value, "\2") != 0 &&
            !(ft_strspaces(current->value) && current->was_quoted != 1))
        {
            char *tmp = ft_strdup(current->value);
            if (!tmp)
                return (0);
            cmd->cmd[arg_i] = remove_char(tmp, '\2');
            free(tmp); // FIX: free intermediate buffer to prevent leak
            if (!cmd->cmd[arg_i])
            {
                while (--arg_i >= 0)
                    free(cmd->cmd[arg_i]);
                free(cmd->cmd);
                cmd->cmd = NULL;
                return 0;
            }
            arg_i++;
        }
        else if (ft_isredirect(current->type))
        {
            if (!current->next || current->next->type != TOKEN_WORD)
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `", 2, 0);
                if (current->next && current->next->value)
                    ft_putstr_fd(current->next->value, 2, 0);
                else
                    ft_putstr_fd("newline", 2, 0);
                ft_putstr_fd("'\n", 2, 0);
                return 0;
            }
            current = current->next;
        }
        current = current->next;
    }
    cmd->cmd[arg_i] = NULL;
    tokens = current;
    return 1;
}

static int parse_redirections(t_cmd *cmd, t_token **tokens)
{
    t_token *current = *tokens;
    t_file *file;

    while (current && current->type != TOKEN_PIPE)
    {
        if (ft_isredirect(current->type))
        {
            if (!current->next || current->next->type != TOKEN_WORD)
            {
                ft_putstr_fd("./minishell: syntax error near unexpected token `", 2, 0);
                if (current->next && current->next->value)
                    ft_putstr_fd(current->next->value, 2, 0);
                else
                    ft_putstr_fd("newline", 2, 0);
                ft_putstr_fd("'\n", 2, 0);
                
                return 0;
            }
            file = init_mfile();
            if (!file)
                return 0;
            char *tmp = ft_strdup(current->next->value);
            if (!tmp)
                 return (free(file), 0);
            file->name = remove_char(tmp , '\2');
            if(!file->name)
                return (free(file), 0);
            file->type = current->type;
            file->check_expand = current->next->was_quoted;
            cmd->files[cmd->file_count++] = *file;
            free(tmp);
            free(file);
            current = current->next;
        }
        current = current->next;
    }
    return 1;
}
//add this just to test it 
static int count_redirections(t_token *start)
{
    int count = 0;
    t_token *current = start;
    while (current && current->type != TOKEN_PIPE)
    {
        if (ft_isredirect(current->type))
            count++;
        current = current->next;
    }
    return count;
}
// --- in parse_single_command ---
static t_cmd *parse_single_command(t_token **tokens)
{
    t_cmd *cmd;
    t_token *start;
    int argc;

    start = *tokens;
    argc = count_args(start);
    cmd = init_cmd();
    if (!cmd)
        return NULL;
    cmd->cmd = malloc(sizeof(char *) * (argc + 1));
    if (!cmd->cmd)
        return (free(cmd), NULL);
    int redir_count = count_redirections(start);
    cmd->files = malloc(sizeof(t_file) * (redir_count + 1));
    if (!cmd->files)
        return (free_array(cmd->cmd), free(cmd), NULL);
    if (!parse_arguments(cmd, *tokens))
        return(free(cmd->files), free(cmd), NULL);
    if (!parse_redirections(cmd, &start))
        return (free_array(cmd->cmd), free(cmd->files), free(cmd), NULL);
    while (*tokens && (*tokens)->type != TOKEN_PIPE)
        *tokens = (*tokens)->next;
    cmd->next = NULL;
    return cmd;
}

t_cmd *parse_commands(t_token *tokens, t_shell *shell_ctx)
{
    t_cmd *cmd_head = NULL;
    t_cmd *current = NULL;
    t_cmd *new_cmd;
    int cmd_count = count_commands(tokens);
    int i = 0;

    while (i < cmd_count)
    {
        new_cmd = parse_single_command(&tokens);
        if (!new_cmd)
            return (free_cmd_list(cmd_head), shell_ctx->exit_status = 258, NULL);
        if (!cmd_head)
            cmd_head = new_cmd;
        else
            current->next = new_cmd;
        current = new_cmd;
        if (tokens && tokens->type == TOKEN_PIPE)
        {
            if (!tokens->next)
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2, 0);
                shell_ctx->exit_status = 258;
                return (free_cmd_list(cmd_head), NULL);
            }
            tokens = tokens->next;
        }
        i++;
    }
    return cmd_head;
}
