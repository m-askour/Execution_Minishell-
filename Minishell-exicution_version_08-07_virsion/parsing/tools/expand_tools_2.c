/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tools_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 21:48:32 by ahari             #+#    #+#             */
/*   Updated: 2025/07/04 00:02:04 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	should_expand_var(char *cmd, int pos)
{
	if (cmd[pos] != '$')
		return (0);
	if (pos > 0 && cmd[pos - 1] == '\\')
		return (0);
	return (1);
}

char	*build_cmd_and_free(char *cmd, int pos, char *val, int skip_len)
{
	char	*new;

	new = build_new_command(cmd, pos, val, skip_len);
	free(val);
	return (new);
}
