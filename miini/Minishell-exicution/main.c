#include "minishell.h"


// this the main is just for testing
int main(int ac,char **av,char **env)
{
    (void)ac;
    (void)av;
    char        *input;
    t_token     *tokens;
    t_cmd       **commands;
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
        tokens = string_tokens(input);
        if (!tokens)
        {
            free(input);
            continue ;
        }
        commands = parse_commands(tokens);
        if (commands)
        {
            int i = 0;
            while (commands[i])
            {
                printf("Command #%d:\n", i + 1);
                print_command_with_files(commands[i]);
                i++;
            }
        }
        if (commands[0])
            exicut_cmd(commands, env);
        free_cmd(*commands);
        free_tokens(tokens, input); 
    }
    return (0);
}

