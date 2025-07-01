// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   process_tokens.c                                   :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/06/27 20:48:17 by ahari             #+#    #+#             */
// /*   Updated: 2025/06/27 21:10:31 by ahari            ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../minishell.h"

// int process_token(t_token *current, t_token **head, t_shell *shell_ctx, char **env_table)
// {     
//     char **new_val = NULL;
//     char *val_cmd = NULL;
//     int i;
//     int is_export_var = 0;
    
//     if (current->type != TOKEN_WORD)         
//         return (1);
//     is_export_var = is_export_assignment(*head, current);
//     new_val = process_quoted_value(current->value, *head, shell_ctx);
//     if (!new_val)         
//         return (0);
//     i = 0;     
//     while(new_val[i])     
//     {         
//         if (!is_single_quoted(current->value, new_val[i]))         
//         {             
//             if (!process_env_expansion(new_val, i, env_table, shell_ctx))
//             {
//                 free_array(new_val);
//                 free_tokens(*head, NULL);
//                 return (0);             
//             }
//             if (!is_quoted(current->value, new_val[i]) && !is_export_var)
//             {
//                 char **split = split_with_quotes(new_val[i]);
//                 if (split && split[0] && split[1])
//                 {
//                     if (!handle_token_splitting(current, head, split))
//                     {
//                         free_array(split);
//                         free_array(new_val);
//                         free_tokens(*head, NULL);
//                         return (0);
//                     }
//                     free_array(split);
//                     free_array(new_val);
//                     return (2);
//                 }
//                 if (split)
//                     free_array(split);
//             }
//             else if (is_export_var && !is_quoted(current->value, new_val[i]))
//             {
//                 if (is_export_var == 1)
//                 {
//                     char **split = split_with_quotes(new_val[i]);
//                     if (split && split[0] && split[1])
//                     {
//                         char *joined = join_export_tokens(split);
//                         if (joined)
//                         {
//                             free(new_val[i]);
//                             new_val[i] = joined;
//                         }
//                         free_array(split);
//                     }
//                 }
//                 else if (is_export_var == 2)
//                 {
//                     char **split = split_with_quotes(new_val[i]);
//                     if (split && split[0] && split[1])
//                     {
//                         char *joined = split[0];
//                         if (joined)
//                             new_val[i] = joined;
//                     }
//                 }
//             }
//         }
        
//         if (new_val[i])
//         {
//             if (i == 0)
//                 val_cmd = ft_strdup(new_val[i]);   
//             else
//             {
//                 char *tmp = ft_strjoin(val_cmd, new_val[i]);
//                 free(val_cmd);             
//                 val_cmd = tmp;             
//                 if (!val_cmd)
//                 {                 
//                     free_array(new_val);                 
//                     free_tokens(*head, NULL);                 
//                     return (0);             
//                 }         
//             }         
//         }
//         i++;     
//     }
    
//     free_array(new_val);
//     if (val_cmd)
//     {
//         free(current->value);     
//         current->value = val_cmd;     
//     }
//     else
//     {
//         free(current->value);
//         current->value = ft_strdup("");
//     }
//     if (!current->value)     
//     {         
//         free_tokens(*head, NULL);         
//         return (0);     
//     }     
//     return (1); 
// }