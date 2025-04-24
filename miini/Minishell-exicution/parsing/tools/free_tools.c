/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:27:19 by ahari             #+#    #+#             */
/*   Updated: 2025/04/21 23:32:03 by ahari            ###   ########.fr       */
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
        free(temp);
    }
    free(input);
}

void free_cmd_list(t_cmd **cmd_list, int count)
{
    int j;
    
    j = 0;
    while (j < count)
    {
        free_cmd(cmd_list[j]);
        j++;
    }
    free(cmd_list);
}