/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 20:55:41 by ahari             #+#    #+#             */
/*   Updated: 2025/04/21 20:59:25 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static const char *redir_type_to_string(t_token_type type)
{
    switch (type)
    {
        case TOKEN_REDIRECT_IN:    return "Input";
        case TOKEN_REDIRECT_OUT:   return "Output";
        case TOKEN_APPEND:         return "Append";
        case TOKEN_HEREDOC:        return "Heredoc";
        default:                   return "Unknown";
    }
}
void print_command_with_files(t_cmd *cmd)
{
    int i;
    
    if (!cmd)
        return;

    // Print command arguments
    printf("Command: ");
    if (cmd->cmd)
    {
        for (i = 0; cmd->cmd[i]; i++)
            printf("[%s] ", cmd->cmd[i]);
    }
    printf("\n");

    // Print file redirections
    if (cmd->files && cmd->file_count > 0)
    {
        printf("Files:\n");
        for (i = 0; i < cmd->file_count; i++)
        {
            printf("  %s: ", redir_type_to_string(cmd->files[i].type));
            printf("%s\n", cmd->files[i].name);
        }
    }
    else
    {
        printf("No file redirections\n");
    }
    printf("--------------------\n");
}

/*-------------------for testing----------------------*/

void print_token_type(t_token_type type)
{
    switch (type)
    {
        case TOKEN_WORD:
            printf("TOKEN_WORD");
            break;
        case TOKEN_PIPE:
            printf("TOKEN_PIPE");
            break;
        case TOKEN_REDIRECT_IN:
            printf("TOKEN_REDIRECT_IN");
            break;
        case TOKEN_REDIRECT_OUT:
            printf("TOKEN_REDIRECT_OUT");
            break;
        case TOKEN_APPEND:
            printf("TOKEN_APPEND");
            break;
        case TOKEN_HEREDOC:
            printf("TOKEN_HEREDOC");
            break;
        case TOKEN_NULL:
            printf("TOKEN_NULL");
            break;
        default:
            printf("UNKNOWN_TOKEN");
            break;
    }
}

void print_tokens(t_token *head)
{
    t_token *current = head;

    while (current)
    {
        printf("Token: %s  Type: ", current->value);
        print_token_type(current->type);
        printf("\n");
        current = current->next;
    }
}