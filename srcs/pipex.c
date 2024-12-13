/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 18:11:14 by ltheveni          #+#    #+#             */
/*   Updated: 2024/12/13 10:27:29 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	init_data(t_pipex *data, int argc, char **argv)
{
	data->pipes = malloc(sizeof(int [2]) * (data->cmd_count - 1));
	if (!data->pipes)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	data->infile = open(argv[1], O_RDONLY);
	if (data->infile < 0)
	{
		perror("open infile");
		exit(EXIT_FAILURE);
	}
	data->outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (data->outfile < 0)
	{
		perror("open outfile");
		exit(EXIT_FAILURE);
	}
}

void	create_pipes(t_pipex *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_count - 1)
	{
		if (pipe(data->pipes[i]) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	wait_for_children(int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count)
	{
		wait(NULL);
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	data;

	if (argc < 5)
	{
		perror("./pipex infile cmd cmd outfile");
		exit(EXIT_FAILURE);
	}
	if (!ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1]) - 1))
	{
		if (argc < 6)
		{
			perror("./pipex infile cmd cmd outfile");
			exit(EXIT_FAILURE);
		}
	}
	data.cmd_count = argc - 3;
	init_data(&data, argc, argv);
	create_pipes(&data);
	fork_processes(argv, envp, &data);
	close_pipes(&data);
	wait_for_children(data.cmd_count);
	close(data.infile);
	close(data.outfile);
	free(data.pipes);
	return (0);
}
