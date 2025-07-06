/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:01:59 by maskour           #+#    #+#             */
/*   Updated: 2025/05/23 20:29:07 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

static void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}
static int	not_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			if (env[i][6] == '\0')
				return (1);
			return (0);
		}
		i++;
	}
	return (1);
}

static char	*get_path_from_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*get_executable_path(char *cmd)
{
	if (access(cmd, F_OK | X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

static char	*search_path_dirs(char *cmd, char **direct)
{
	char	*full_path;
	char	*dir_with_slash;
	int		i;

	i = -1;
	while (direct[++i])
	{
		dir_with_slash = ft_strjoin(direct[i], "/");
		if (!dir_with_slash)
			return (NULL);
		full_path = ft_strjoin(dir_with_slash, cmd);
		free(dir_with_slash);
		if (!full_path)
			return (NULL);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
	}
	return (NULL);
}

char	*find_path(char *cmd, char **env)
{
	char	*path_env;
	char	**direct;
	char	*result;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/') != NULL || not_path(env))
		return (get_executable_path(cmd));
	path_env = get_path_from_env(env);
	if (!path_env)
		return (NULL);
	direct = ft_split_up(path_env, ':');
	if (!direct)
		return (NULL);
	result = search_path_dirs(cmd, direct);
	free_split(direct);
	return (result);
}
