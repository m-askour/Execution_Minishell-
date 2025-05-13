#include "minishell.h"


// this the main is just for testing
int main(int ac,char **av,char **env)
{
    (void)ac;
    (void)av;
    char        *input;
    t_token     *tokens;
    t_cmd       *commands;
    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
        {
            printf("exit\n");
            break ;
        }
        if (*input)
            add_history(input);
        tokens = check_quoted(input);
        if (!tokens)
            continue ;
        commands = parse_commands(tokens);
        commands = expand_cmd_list(commands);
        // if (commands)
        // {
        //     t_cmd *current = commands;
        //     int i = 1;
            
        //     while (current)
        //     {
        //         printf("Command #%d:\n", i);
        //         print_command_with_files(current);
        //         current = current->next;
        //         i++;
        //     }
        // }
        exicut(&commands, env);
        //  free_cmd(*commands);
        free_tokens(tokens, input);

    }
    return (0);
}

