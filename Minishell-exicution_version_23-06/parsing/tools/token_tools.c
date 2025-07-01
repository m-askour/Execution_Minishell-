// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   token_tools.c                                      :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/06/27 21:11:58 by ahari             #+#    #+#             */
// /*   Updated: 2025/06/27 21:12:28 by ahari            ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../../minishell.h"

// int is_single_quoted(char *original_val, char *substring)
// {
//     char *pos = ft_strstr(original_val, substring);
//     int quote_count = 0;
//     int i = 0;
    
//     if (!pos)
//         return 0;
    
//     while (&original_val[i] < pos)
//     {
//         if (original_val[i] == '\'' && (i == 0 || original_val[i - 1] != '\\'))
//             quote_count++;
//         i++;
//     }
//     return (quote_count % 2 == 1);
// }

// int is_quoted(char *original_val, char *substring)
// {
//     char *pos = ft_strstr(original_val, substring);
//     int i = 0;
//     char current_quote = 0;
    
//     if (!pos)
//         return 0;
//     while (&original_val[i] < pos)
//     {
//         if ((original_val[i] == '\'' || original_val[i] == '"') && 
//             (i == 0 || original_val[i - 1] != '\\'))
//         {
//             if (!current_quote)
//                 current_quote = original_val[i];
//             else if (original_val[i] == current_quote)
//                 current_quote = 0;
//         }
//         i++;
//     }
//     return (current_quote != 0);
// }
