/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:12:45 by ltheveni          #+#    #+#             */
/*   Updated: 2024/12/16 09:20:11 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	redirect_first_pipe(int i, t_pipex *data)
{
	if (data->is_here_doc || (data->infile > 0 && !data->is_here_doc))
	{
		if (dup2(data->infile, STDIN_FILENO) == -1)
			perror_exit("dup2 infile");
	}
	if (dup2(data->pipes[i][1], STDOUT_FILENO) == -1)
		perror_exit("dup2 pipe write end");
}

void	redirect_last_pipe(int i, t_pipex *data)
{
	if (dup2(data->pipes[i - 1][0], STDIN_FILENO) == -1)
		perror_exit("dup2 pipe read end");
	if (data->outfile != -1)
	{
		if (dup2(data->outfile, STDOUT_FILENO) == -1)
			perror_exit("dup2 outfile");
	}
}

void	redirect_remains(int i, t_pipex *data)
{
	if (dup2(data->pipes[i - 1][0], STDIN_FILENO) == -1)
		perror_exit("dup2 pipe read end");
	if (dup2(data->pipes[i][1], STDOUT_FILENO) == -1)
		perror_exit("dup2 pipe wirte end");
}

void	redirect(int i, t_pipex *data)
{
	if (i == 0)
		redirect_first_pipe(i, data);
	else if (i == data->cmd_count - 1)
		redirect_last_pipe(i, data);
	else
		redirect_remains(i, data);
}
