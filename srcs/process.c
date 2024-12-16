/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:32:44 by ltheveni          #+#    #+#             */
/*   Updated: 2024/12/16 21:13:11 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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
	if (i == 0 && data->infile < 0 && !data->is_here_doc)
	{
		if (data->infile == -1)
			perror("Permission denied (infile read)");
		free(data->pipes);
		exit(EXIT_FAILURE);
	}
	else if (i == data->cmd_count - 1 && data->outfile == -1)
	{
		perror("Permission denied (outfile write)");
		free(data->pipes);
		exit(EXIT_FAILURE);
	}
	exec_cmd(i, argv, envp, data);
	free(data->pipes);
	exit(EXIT_FAILURE);
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
		if (i < data->cmd_count - 1)
			close(data->pipes[i][1]);
		if (i > 0)
			close(data->pipes[i - 1][0]);
		i++;
	}
}
