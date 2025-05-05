/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:43:32 by ahari             #+#    #+#             */
/*   Updated: 2025/04/24 17:11:25 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int check_senbol(char c)
{
    if (c == '$')
        return (1);
    return (0);
}

char *ft_strstr(const char *haystack, const char *needle) {
    if (!*needle) // Empty needle
        return (char *)haystack;

    while (*haystack) {
        const char *h = haystack;
        const char *n = needle;

        while (*h && *n && *h == *n) {
            h++;
            n++;
        }

        if (!*n) // If we reached the end of needle, match found
            return (char *)haystack;

        haystack++;
    }

    return NULL;
}

int main(void) {
    const char *text = "Hello, $USER students!";
    const char *find = "USER";

    char *res = ft_strstr(text, find);
    if (res)
        printf("Found: %s\n", res);
    else
        printf("Not found\n");

    return 0;
}