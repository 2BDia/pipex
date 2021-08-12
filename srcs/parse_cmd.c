/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 18:38:33 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/12 14:40:55 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	count_args(char *arg)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (arg[i])
	{
		if ((is_space(arg[i + 1]) || arg[i + 1] == '\0') && !is_space(arg[i]))
			count++;
		i++;
	}
	return (count);
}

char	**parse_cmd(char **argv, int mod)
{
	char	**cmd;
	int		n;

	cmd = (char **)malloc(sizeof(char *) * (count_args(argv[mod]) + 1));
	if (!cmd)
		return (NULL);
	cmd[0] = cut_cmd_flags(argv, cmd, mod);
	if (!cmd[0])
	{
		free(cmd);
		return (NULL);
	}
	if (cmd[1])
	{
		n = cut_flags_args(cmd);
		if (n == -1)
		{
			while (n > 0)
				free(cmd[n--]);
			free(cmd);
			return (NULL);
		}
		cmd[n] = NULL;
	}
	return (cmd);
}
