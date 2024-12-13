/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 17:29:04 by ltheveni          #+#    #+#             */
/*   Updated: 2024/12/13 23:36:14 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	handle_quote(const char *cmd, int i)
{
	char	quote;

	quote = cmd[i++];
	while (cmd[i] && cmd[i] != quote)
		i++;
	if (cmd[i] == quote)
		i++;
	return (i);
}

int	count_cmd(const char *cmd)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (cmd[i])
	{
		while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t' || cmd[i] == '\n'))
			i++;
		if (cmd[i] == '"' || cmd[i] == '\'')
		{
			count++;
			i = handle_quote(cmd, i);
		}
		else if (cmd[i])
		{
			count++;
			while (cmd[i] && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '\n')
				i++;
		}
	}
	return (count);
}

void	handle_cmd_quote(char const *cmd, char **result, int *i, int *j)
{
	int		start;
	char	quote;

	quote = cmd[(*i)++];
	start = *i;
	while (cmd[*i] && cmd[*i] != quote)
		(*i)++;
	result[(*j)++] = ft_substr(cmd, start, *i - start);
	if (cmd[*i] == quote)
		(*i)++;
}

int	handle_cmd(const char *cmd, char **result)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	while (cmd[i])
	{
		while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t' || cmd[i] == '\n'))
			i++;
		if (cmd[i] == '"' || cmd[i] == '\'')
			handle_cmd_quote(cmd, result, &i, &j);
		else if (cmd[i])
		{
			start = i;
			while (cmd[i] && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '\n')
				i++;
			result[j++] = ft_substr(cmd, start, i - start);
		}
	}
	return (j);
}

char	**parse_cmd(const char *cmd)
{
	int		i;
	char	**result;

	if (!is_quote(cmd))
		return (ft_split(cmd, ' '));
	result = (char **)malloc(sizeof(char *) * (count_cmd(cmd) + 1));
	if (!result)
		return (NULL);
	i = handle_cmd(cmd, result);
	result[i] = NULL;
	return (result);
}
