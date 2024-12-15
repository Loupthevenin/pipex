/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 14:27:00 by ltheveni          #+#    #+#             */
/*   Updated: 2024/12/15 08:58:42 by ltheveni         ###   ########.fr       */
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

void	handle_file_error(int *file, int flags)
{
	*file = open("/dev/null", flags);
	if (*file < 0)
		perror_exit("open /dev/null");
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
