/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 14:40:59 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/12 14:41:44 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	put_in_cmd1(char **argv, char **cmd, int mod, int *cursor)
{
	while (is_space(argv[mod][++(cursor[0])]))
		(cursor[1])++;
	while (argv[mod][cursor[1]])
		(cursor[1])++;
	cmd[1] = (char *)malloc(sizeof(char) * ((cursor[1] - cursor[0]) + 1));
	if (!cmd[1])
	{
		cmd[0] = NULL;
		return ;
	}
	cursor[1] = 0;
	while (argv[mod][cursor[0]])
		cmd[1][(cursor[1])++] = argv[mod][(cursor[0])++];
	cmd[1][cursor[1]] = '\0';
	cursor[0] = 0;
	if (ft_strlen(cmd[1]) == 0)
	{
		free(cmd[1]);
		cmd[1] = NULL;
	}
}

char	*cut_cmd_flags(char **argv, char **cmd, int mod)
{
	char	*str;
	int		len;
	int		i;
	int		cursor[2];

	len = 0;
	i = 0;
	while (argv[mod][len] && !is_space(argv[mod][len]))
		len++;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (argv[mod][i] && !is_space(argv[mod][i]))
	{
		str[i] = argv[mod][i];
		i++;
	}
	str[i] = '\0';
	i--;
	cursor[0] = i;
	cursor[1] = len;
	put_in_cmd1(argv, cmd, mod, cursor);
	return (str);
}

static int	cut_flags_args_loop(char *tmp, char **cmd, int n, int i)
{
	int	len;
	int	icpy;

	while (tmp[i])
	{
		len = 0;
		if (!is_space(tmp[i]))
		{
			icpy = i;
			while (!is_space(tmp[i]) && tmp[i] && ++i)
				len++;
			cmd[n] = (char *)malloc(sizeof(char) * (len + 1));
			if (!cmd[n])
				return (-1);
			len = 0;
			while (!is_space(tmp[icpy]) && tmp[icpy])
				cmd[n][len++] = tmp[icpy++];
			cmd[n][len] = '\0';
		}
		while (is_space(tmp[i]))
			i++;
		n++;
	}
	return (n);
}

int	cut_flags_args(char **cmd)
{
	int		i;
	int		n;
	char	*tmp;

	i = 0;
	n = 1;
	tmp = ft_strdup(cmd[1]);
	free(cmd[1]);
	n = cut_flags_args_loop(tmp, cmd, n, i);
	free(tmp);
	return (n);
}
