/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 11:12:45 by ltheveni          #+#    #+#             */
/*   Updated: 2024/12/13 08:57:35 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	**get_path_dir(char **envp)
{
	int		i;
	char	*path_value;

	i = 0;
	path_value = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_value = envp[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_value)
		return (NULL);
	return (ft_split(path_value, ':'));
}

int	is_executable(const char *path)
{
	return (access(path, X_OK) == 0);
}

char	*join_path(const char *dir, const char *cmd)
{
	char	*full_path;
	size_t	len;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	full_path = (char *)malloc(sizeof(char) * (len + 1));
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, dir, len);
	ft_strlcat(full_path, "/", len);
	ft_strlcat(full_path, cmd, len);
	return (full_path);
}

char	*get_cmd_path(const char *cmd, char **envp)
{
	char	**dirs;
	char	*full_path;
	int		i;

	if (ft_strchr(cmd, '/'))
	{
		if (is_executable(cmd))
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	dirs = get_path_dir(envp);
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		full_path = join_path(dirs[i], cmd);
		if (!full_path)
		{
			perror("malloc");
			break ;
		}
		if (is_executable(full_path))
		{
			free_tab(dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_tab(dirs);
	return (NULL);
}

void	exec_cmd(int i, char **argv, char **envp)
{
	char	**cmd;
	char	*cmd_path;

	cmd = ft_split(argv[i + 2], ' ');
	if (!cmd | !cmd[0])
	{
		perror("Invalid Command");
		exit(EXIT_FAILURE);
	}
	/* ft_printf("%s\n", cmd[0]); */
	/* ft_printf("%s\n", cmd[1]); */
	cmd_path = get_cmd_path(cmd[0], envp);
	if (!cmd_path)
	{
		free_tab(cmd);
		perror("Command not found");
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, cmd, envp);
	free(cmd_path);
	free_tab(cmd);
	perror("execve");
	exit(EXIT_FAILURE);
}
