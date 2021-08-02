/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 13:05:57 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/02 17:53:46 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*cut_cmd_flags(char **argv, char **cmd)
{
	char	*str;
	int		len;
	int		i;

	len = 0;
	i = 0;
	while (argv[2][len] && argv[2][len] != ' ')
		len++;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (argv[2][i] && argv[2][i] != ' ')
	{
		str[i] = argv[2][i];
		i++;
	}
	str[i] = '\0';
	i--;
	while (argv[2][++i] == ' ')
		len++;
	while (argv[2][len])
		len++;
	cmd[1] = (char *)malloc(sizeof(char) * ((len - i) + 1));
	if (!cmd[1])
		return (NULL);
	len = 0;
	while (argv[2][i])
		cmd[1][len++] = argv[2][i++];
	cmd[1][i] = '\0';
	return (str);
}

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

static char	**split_paths(char *env)
{
	char	**path;
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	j = 5;
	path = ft_split(env, ':');
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

int	main(int argc, char **argv, char **env)
{
	char		**cmd;
	char		**path;
	char		*tmp;
	int			i;

	i = 0;
	if (argc != 5)
		return (0);
	cmd = (char **)malloc(sizeof(char *) * 3);
	cmd[0] = cut_cmd_flags(argv, cmd);
	// if (!cmd[0])
	// 	error free etc
	cmd[2] = NULL;
	path = split_paths(env[6]); //to do : trouver la ligne de PATH=
	tmp = ft_strdup(cmd[0]);
	if (fork() == 0) //Child
	{
		while (execve(*cmd, cmd, env) == -1)
		{
			free(cmd[0]);
			cmd[0] = ft_strjoin(path[i], tmp);
			i++;
		}
	}
	free(tmp);
	free_arrays(cmd);
	free_arrays(path);
	system("leaks a.out");
	return (0);
}
















// char		*cmd[] = {"/bin/ls", "-la", (void *)0};

// int main(int argc, char **argv, char **env)
// {
// 	char *coucou[] = {"/bin/echo", "-n", "hello", (void *)0};
// 	execve(*coucou, coucou, env);
// }
// while (execve() == -1)
	//cat(chemin + commande)
//execve(char *commande, char **cmd, char **environnement)
//char **cmd = {"grep", "-n", "q"}


// /Users/rvan-aud/.brew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki