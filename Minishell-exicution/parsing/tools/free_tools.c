/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:27:19 by ahari             #+#    #+#             */
/*   Updated: 2025/04/22 18:49:27 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void    free_files(t_file *files, int file_count)
{
    int i;

    if (!files)
        return;
    
    i = 0;
    while (i < file_count)
    {
        free(files[i].name);
        files[i].name = NULL;
        i++;
    }
    free(files);
    files = NULL;
}

void    free_cmd_array(char **cmd)
{
    int i;

    if (!cmd)
        return;
    i = 0;
    while (cmd[i])
    {
        free(cmd[i]);
        cmd[i] = NULL;
        i++;
    }
    free(cmd);
    cmd = NULL;
}

void    free_cmd(t_cmd *cmd)
{
    if (!cmd)
        return;
    free_cmd_array(cmd->cmd);
    free_files(cmd->files, cmd->file_count);
    free(cmd);
}

void free_tokens(t_token *tokens, char *input)
{
    t_token *temp;

    while (tokens)
    {
        temp = tokens;
        tokens = tokens->next;
        free(temp->value);
        temp->value = NULL;
        free(temp);
        temp = NULL;
    }
    if (input)
        free(input);
    input = NULL;
}

void free_cmd_list(t_cmd *cmd_head)
{
    t_cmd *current;
    t_cmd *next;
    
    current = cmd_head;
    while (current)
    {
        next = current->next;  // Save the next pointer before freeing current
        free_cmd(current);     // Free the current command
        current = next;        // Move to the next command
    }
}