/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 13:30:11 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/03 13:30:47 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*add_forw_slash(char *str)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = ft_strdup(str);
	free(str);
	str = (char *)malloc(sizeof(char) * (ft_strlen(tmp) + 2));
	while (tmp[i])
	{
		str[i] = tmp[i];
		i++;
	}
	str[i++] = '/';
	str[i] = '\0';
	free(tmp);
	return (str);
}

static int	check_path_line(char **env)
{
	int	i;

	i = 0;
	while (ft_strncmp(env[i], "PATH=", 5))
		i++;
	return (i);
}

char	**split_paths(char **env)
{
	char	**path;
	char	*tmp;
	int		i;
	int		j;
	int		p_line;

	i = 0;
	j = 5;
	p_line = check_path_line(env);
	path = ft_split(env[p_line], ':');
	tmp = ft_strdup(path[0]);
	free(path[0]);
	path[0] = (char *)malloc(sizeof(char) * ((ft_strlen(tmp) - 5) + 1));
	while (tmp[j])
		path[0][i++] = tmp[j++];
	path[0][i] = '\0';
	free(tmp);
	i = 0;
	while (path[i])
	{
		tmp = ft_strdup(path[i]);
		free(path[i]);
		path[i] = add_forw_slash(tmp);
		i++;
	}
	return (path);
}
