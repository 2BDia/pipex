/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 13:05:57 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/03 16:35:17 by rvan-aud         ###   ########.fr       */
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
	
	while (argv[2][len] && !is_space(argv[2][len]))
		len++;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (argv[2][i] && !is_space(argv[2][i]))
	{
		str[i] = argv[2][i];
		i++;
	}
	str[i] = '\0';
	i--;
	while (is_space(argv[2][++i]))
		len++;
	while (argv[2][len])
		len++;
	cmd[1] = (char *)malloc(sizeof(char) * ((len - i) + 1));
	if (!cmd[1])
		return (NULL);
	len = 0;
	while (argv[2][i])
		cmd[1][len++] = argv[2][i++];
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

int	main(int argc, char **argv, char **env)
{
	char		**cmd;
	char		**path;
	char		*tmp;
	int			i;
	int			n;

	i = 0;
	if (argc != 5)
		return (0);
	cmd = (char **)malloc(sizeof(char *) * (count_args(argv[2]) + 1));
	cmd[0] = cut_cmd_flags(argv, cmd);
	// if (!cmd[0])
	// 	error free etc
	if (cmd[1])
	{
		n = cut_flags_args(cmd);
		// if (n == -1)
		// 	error free etc
		cmd[n] = NULL;
	}
	// printf("cmd[0]=%s\n", cmd[0]);
	// printf("cmd[1]=%s\n", cmd[1]);
	// printf("cmd[2]=%s\n", cmd[2]);
	// printf("cmd[3]=%s\n", cmd[3]);
	path = split_paths(env);
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
	// system("leaks pipex");
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