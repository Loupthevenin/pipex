/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:32:44 by ltheveni          #+#    #+#             */
/*   Updated: 2024/12/14 17:05:39 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	redirect(int i, t_pipex *data)
{
	if (i == 0)
	{
		if (dup2(data->infile, STDIN_FILENO) == -1)
			perror_exit("dup2 infile");
		if (dup2(data->pipes[i][1], STDOUT_FILENO) == -1)
			perror_exit("dup2 pipe write end");
	}
	else if (i == data->cmd_count - 1)
	{
		if (dup2(data->pipes[i - 1][0], STDIN_FILENO) == -1)
			perror_exit("dup2 pipe read end");
		if (dup2(data->outfile, STDOUT_FILENO) == -1)
			perror_exit("dup2 outfile");
	}
	else
	{
		if (dup2(data->pipes[i - 1][0], STDIN_FILENO) == -1)
			perror_exit("dup2 pipe read end");
		if (dup2(data->pipes[i][1], STDOUT_FILENO) == -1)
			perror_exit("dup2 pipe wirte end");
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
			perror_exit("fork");
		if (pid == 0)
		{
			process(i, argv, envp, data);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}
