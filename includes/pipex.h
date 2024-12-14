/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 18:15:40 by ltheveni          #+#    #+#             */
/*   Updated: 2024/12/14 15:27:23 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "ft_printf.h"
# include "libft.h"
# include <fcntl.h>
# include <sys/wait.h>

typedef struct s_pipex
{
	int	infile;
	int	outfile;
	int	(*pipes)[2];
	int	cmd_count;
	int	is_here_doc;
}		t_pipex;

int		is_executable(const char *path);
int		is_quote(const char *cmd);
void	wait_for_children(int cmd_count);
void	perror_exit(const char *msg);
void	fork_processes(char **argv, char **envp, t_pipex *data);
void	close_pipes(t_pipex *data);
void	free_tab(char **dirs);
char	**parse_cmd(const char *cmd);
int		has_escape(const char *str);
void	exec_cmd(int i, char **argv, char **envp, t_pipex *data);

#endif
