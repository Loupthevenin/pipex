/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 14:27:00 by ltheveni          #+#    #+#             */
/*   Updated: 2024/12/17 13:43:03 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_tab(char **tabs)
{
	int	i;

	if (!tabs)
		return ;
	i = 0;
	while (tabs[i])
	{
		free(tabs[i]);
		i++;
	}
	free(tabs);
}

void	cleanup_exec_cmd(t_pipex *data, char **cmd)
{
	free_tab(cmd);
	free(data->pipes);
	if (data->infile >= 0)
		close(data->infile);
	if (data->outfile >= 0)
		close(data->outfile);
}

int	is_quote(const char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '"' || cmd[i] == '\'')
			return (1);
		i++;
	}
	return (0);
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

void	perror_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
