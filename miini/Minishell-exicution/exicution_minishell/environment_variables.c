/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_variables.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:11:14 by maskour           #+#    #+#             */
/*   Updated: 2025/04/20 22:44:27 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//expand $0;
char *expand_0(void)
{
    return("./minishell");
}
//expand $$

char *expand_pip(void)
{
    pid_t pid;
    char *pid_str;
    
    pid = get_pid();
    pid_str = ft_itoa(pid); // this to convert the pid to string;
    return (pid_str);
}

//expand $? // it 
// char *expand_exit_stat(void)
// {
//     char *status_str;
//     status_str = ft_itoa();
//     return (status_str);
// }


char *expand_env_var(t_token *args)
{
    if (!args || args != "$")
        return(ft_strdup(args));
    args++;
    if (ft_strcmp(args, "$") == 0)
        expand_exit_stat();
    else if(ft_strcmp(args, "0") == 0)
        expand_0();
    else if(ft_stcmp(args, "?") == 0) //stile this i don´tknow how to complet it 
        expand_pip();

    char *val;
    val = getenv(args);
    if (!val)
        return (ft_strdup(" "));
    return(ft_strdup(val));
}
void expand_tokens(t_token *tokens)
{
    t_token *current;
    char *expanded;
    current = tokens;
    while (current != NULL)
    {
        if(ft_strchr(current->value, '$'))  
        {
            expanded = expand_env_var(current->value);
            free(current->value);
            current->value = expanded;
        } 
     current = current->next;  
    }
}