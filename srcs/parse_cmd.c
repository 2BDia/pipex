/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 18:38:33 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/11 18:34:10 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*cut_cmd_flags(char **argv, char **cmd, int mod)
{
	char	*str;
	int		len;
	int		i;

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
	while (is_space(argv[mod][++i]))
		len++;
	while (argv[mod][len])
		len++;
	cmd[1] = (char *)malloc(sizeof(char) * ((len - i) + 1));
	if (!cmd[1])
		return (NULL);
	len = 0;
	while (argv[mod][i])
		cmd[1][len++] = argv[mod][i++];
	cmd[1][len] = '\0';
	i = 0;
	if (ft_strlen(cmd[1]) == 0)
	{
		free(cmd[1]);
		cmd[1] = NULL;
	}
	return (str);
}

static int	cut_flags_args(char **cmd)
{
	int		i;
	int		icpy;
	int		len;
	int		n;
	char	*tmp;

	i = 0;
	n = 1;
	tmp = ft_strdup(cmd[1]);
	free(cmd[1]);
	while (tmp[i])
	{
		len = 0;
		if (!is_space(tmp[i]))
		{
			icpy = i;
			while (!is_space(tmp[i]) && tmp[i])
			{
				len++;
				i++;
			}
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
	free(tmp);
	return (n);
}

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