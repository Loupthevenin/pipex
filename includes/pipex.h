/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 18:15:40 by ltheveni          #+#    #+#             */
/*   Updated: 2024/12/12 19:35:24 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "ft_printf.h"
# include "libft.h"
# include <fcntl.h>

typedef struct s_pipex
{
	int	infile;
	int	outfile;
	int	here_doc;
	int (*pipes)[2];
	int	cmd_count;
}		t_pipex;

void	fork_processes(char **argv, char **envp, t_pipex *data);
void	close_pipes(t_pipex *data);
void	free_tab(char **dirs);
void	exec_cmd(int i, char **argv, char **envp);

#endif
