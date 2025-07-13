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

#include "../../minishell.h"

int has_quotes(char *str)
{
	int i;

    i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			return (1);
		if (str[i] == '\"')
			return (2);
		if (str[i] == '$' || ft_isdigit(str[i]))
			return (3);
		if (str[i] == '=')
			break ;
		i++;
	}
	return (0);
}

t_token	*get_cmd_token(t_token *head, t_token *current)
{
	t_token *temp;
	t_token *cmd_token;

    temp = head;
    cmd_token = NULL;
	while (temp && temp != current)
	{
		if (temp->type == TOKEN_PIPE || temp->type == TOKEN_SEMICOLON)
			cmd_token = NULL;
		else if (temp->type == TOKEN_WORD && !cmd_token)
			cmd_token = temp;
		temp = temp->next;
	}
	return (cmd_token);
}

int count_herdoc(t_cmd *cmd)
{
    int count = 0;
    int i= 0;
    while (i < cmd->file_count) 
    {
        if (cmd->files->type == TOKEN_HEREDOC)
                count++;
        i++;
    }
    return count;
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	if (!dst && !src)
		return (NULL);
	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

void *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
    void *new_ptr;
    if (new_size == 0)
    {
        free(ptr);
        return NULL;
    }
    if (!ptr)
        return malloc(new_size);

    new_ptr = malloc(new_size);
    if (!new_ptr)
        return NULL;
    if (old_size > new_size)
        old_size = new_size;
    ft_memcpy(new_ptr, ptr, old_size);
    free(ptr);
    return new_ptr;
}