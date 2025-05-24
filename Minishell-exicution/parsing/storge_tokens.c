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

static int count_redirections(t_token *tokens)
{
    int count = 0;
    while (tokens && tokens->type != TOKEN_PIPE)
    {
        if (ft_isredirect(tokens->type))
            count++;
        tokens = tokens->next;
    }
    return count;
}

static int parse_arguments(t_cmd *cmd, t_token **tokens)
{
    int arg_i = 0;
    t_token *current = *tokens;

    while (current && current->type != TOKEN_PIPE)
    {
        if (current->type == TOKEN_WORD)
            cmd->cmd[arg_i++] = ft_strdup(current->value);
        else if (ft_isredirect(current->type))
        {
            if (!current->next || current->next->type != TOKEN_WORD)
            {
                ft_putstr_fd("bash: syntax error near unexpected token ` \n", 2, 0);
                if (current->next && current->next->value)
                    ft_putstr_fd(current->next->value, 2, 0);
                else
                    ft_putstr_fd("newline \n", 2, 0);
                return 0;
            }
            current = current->next;
        }
        current = current->next;
    }
    cmd->cmd[arg_i] = NULL;
    *tokens = current;
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
            file->name = ft_strdup(current->next->value);
            file->type = current->type;
            if (!file->name)
            {
                free(file);
                return 0;
            }
            cmd->files[cmd->file_count++] = *file;
            free(file);
            current = current->next;
        }
        current = current->next;
    }
    return 1;
}

static t_cmd *parse_single_command(t_token **tokens)
{
    t_cmd *cmd;
    t_token *start = *tokens;
    int argc = count_args(start);
    int redir_count = count_redirections(start);

    cmd = init_cmd();
    if (!cmd)
        return NULL;

    cmd->cmd = malloc(sizeof(char *) * (argc + 1));
    if (!cmd->cmd)
        return (free_cmd(cmd), NULL);

    cmd->files = malloc(sizeof(t_file) * (redir_count + 1));
    if (!cmd->files)
        return (free_cmd(cmd), NULL);

    if (!parse_arguments(cmd, tokens) || !parse_redirections(cmd, &start))
        return (free_cmd(cmd), NULL);

    while (*tokens && (*tokens)->type != TOKEN_PIPE)
        *tokens = (*tokens)->next;

    cmd->next = NULL;
    return cmd;
}

t_cmd *parse_commands(t_token *tokens)
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
        {
            free_cmd_list(cmd_head);
            return NULL;
        }

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
                free_cmd_list(cmd_head);
                return NULL;
            }
            tokens = tokens->next;
        }
        i++;
    }

    return cmd_head;
}
