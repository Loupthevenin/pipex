/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 18:11:14 by ltheveni          #+#    #+#             */
/*   Updated: 2024/12/15 09:41:15 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	init_data(t_pipex *data, int argc, char **argv)
{
	data->pipes = malloc(sizeof(int [2]) * (data->cmd_count - 1));
	if (!data->pipes)
		perror_exit("malloc");
	if (!data->is_here_doc)
	{
		data->infile = open(argv[1], O_RDONLY);
		if (data->infile < 0)
			handle_file_error(&data->infile, O_RDONLY);
		data->outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	}
	else
		data->outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND,
				0644);
	if (data->outfile < 0)
		handle_file_error(&data->outfile, O_WRONLY);
}

void	create_pipes(t_pipex *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_count - 1)
	{
		if (pipe(data->pipes[i]) == -1)
			perror_exit("pipe");
		i++;
	}
}

int	is_here_doc(char *str)
{
	if (!ft_strncmp(str, "here_doc", ft_strlen(str)))
		return (1);
	else
		return (0);
}

void	handle_here_doc(t_pipex *data, char *delimiter)
{
	char	*line;
	int		fd;

	fd = open(".here_doc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		perror_exit("open temp file");
	while (1)
	{
		ft_putstr_fd("here_doc> ", 1);
		line = get_next_line(0);
		if (!line || !ft_strncmp(line, delimiter, ft_strlen(delimiter)))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		free(line);
	}
	close(fd);
	data->infile = open(".here_doc_tmp", O_RDONLY);
	if (data->infile < 0)
		perror_exit("open temp file");
	unlink(".here_doc_tmp");
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	data;

	if (argc < 5)
		perror_exit("./pipex infile cmd cmd outfile");
	data.is_here_doc = is_here_doc(argv[1]);
	if (data.is_here_doc && argc < 6)
		perror_exit("./pipex here_doc LIMITER cmd cmd outfile");
	data.cmd_count = argc - 3 - data.is_here_doc;
	init_data(&data, argc, argv);
	if (data.is_here_doc)
		handle_here_doc(&data, argv[2]);
	create_pipes(&data);
	fork_processes(argv, envp, &data);
	close_pipes(&data);
	wait_for_children(data.cmd_count);
	close(data.infile);
	close(data.outfile);
	free(data.pipes);
	return (0);
}
