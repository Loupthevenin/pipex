/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:32:44 by ltheveni          #+#    #+#             */
/*   Updated: 2024/12/11 16:44:25 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	redirect(int i, t_pipex *data)
{
	if (i == 0)
	{
		dup2(data->infile, STDIN_FILENO);
		dup2(data->pipes[i][1], STDOUT_FILENO);
	}
	else if (i == data->cmd_count - 1)
	{
		dup2(data->pipes[i - 1][0], STDIN_FILENO);
		dup2(data->outfile, STDOUT_FILENO);
	}
	else
	{
		dup2(data->pipes[i - 1][0], STDIN_FILENO);
		dup2(data->pipes[i][1], STDOUT_FILENO);
	}
}

void	close_pipes(t_pipex *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_count - 1)
	{
		close(data->pipes[i][0]);
		close(data->pipes[i][1]);
		i++;
	}
}

void	process(int i, char **argv, char **envp, t_pipex *data)
{
	redirect(i, data);
	close_pipes(data);
	exec_cmd(i, argv, envp, data);
}

void	fork_processes(char **argv, char **envp, t_pipex *data)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < data->cmd_count)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
			process(i, argv, envp, data);
		i++;
	}
}
