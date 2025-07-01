/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 13:38:51 by ahari             #+#    #+#             */
/*   Updated: 2025/06/25 13:57:21 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char    *unquoted_string(char *str)
{
    char *result;
    int i = 0;
    int j = 0;

    if (!str)
        return NULL;
    result = malloc(strlen(str) + 1);
    if (!result)
        return NULL;
    while (str[i])
    {
        if (str[i] != '\'' && str[i] != '\"')
            result[j++] = str[i];
        i++;
    }
    result[j] = '\0';
    return result;
}

