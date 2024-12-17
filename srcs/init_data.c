/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 14:50:37 by ltheveni          #+#    #+#             */
/*   Updated: 2024/12/17 09:28:37 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	check_file_permission(const char *file, int mode)
{
	if (access(file, F_OK) != 0 && mode == 0)
	{
		perror(file);
		return (-2);
	}
	else if (access(file, F_OK) != 0 && mode == 1)
		return (0);
	if (mode == 0)
	{
		if (access(file, R_OK) != 0)
			return (-1);
	}
	else if (mode == 1)
	{
		if (access(file, W_OK) != 0)
			return (-1);
	}
	return (0);
}

int	open_file(const char *file, int flags, int mode)
{
	int	fd;
	int	check_error;

	check_error = check_file_permission(file, mode);
	if (check_error < 0)
		return (check_error);
	fd = open(file, flags, 0644);
	if (fd < 0)
		perror(file);
	return (fd);
}

int	setup_infile(const char *infile, int is_here_doc)
{
	if (is_here_doc)
		return (-1);
	return (open_file(infile, O_RDONLY, 0));
}

int	setup_outfile(const char *outfile, int is_here_doc)
{
	int	flags;

	if (is_here_doc)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	return (open_file(outfile, flags, 1));
}

void	init_data(t_pipex *data, int argc, char **argv)
{
	data->pipes = malloc(sizeof(int [2]) * (data->cmd_count - 1));
	if (!data->pipes)
		perror_exit("malloc");
	data->infile = setup_infile(argv[1], data->is_here_doc);
	data->outfile = setup_outfile(argv[argc - 1], data->is_here_doc);
}
