/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 11:12:45 by ltheveni          #+#    #+#             */
/*   Updated: 2024/12/15 09:25:37 by ltheveni         ###   ########.fr       */
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

char	*loop_dirs(char **dirs, const char *cmd)
{
	int		i;
	char	*full_path;

	i = 0;
	while (dirs[i])
	{
		full_path = join_path(dirs[i], cmd);
		if (!full_path)
		{
			perror("malloc");
			break ;
		}
		if (access(full_path, X_OK) == 0)
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

char	*get_cmd_path(const char *cmd, char **envp)
{
	char	**dirs;
	char	*result;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	dirs = get_path_dir(envp);
	if (!dirs)
		return (NULL);
	result = loop_dirs(dirs, cmd);
	if (!result)
		return (NULL);
	return (result);
}

void	exec_cmd(int i, char **argv, char **envp, t_pipex *data)
{
	char	**cmd;
	char	*cmd_path;

	cmd = parse_cmd(argv[i + 2 + data->is_here_doc]);
	if (!cmd | !cmd[0])
	{
		free_tab(cmd);
		free(data->pipes);
		perror_exit("Invalid Command");
	}
	cmd_path = get_cmd_path(cmd[0], envp);
	if (!cmd_path)
	{
		free_tab(cmd);
		free(data->pipes);
		perror_exit("Command not found");
	}
	execve(cmd_path, cmd, envp);
	free(cmd_path);
	free_tab(cmd);
	perror_exit("execve");
}
