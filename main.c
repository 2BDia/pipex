/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 13:05:57 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/03 14:18:14 by rvan-aud         ###   ########.fr       */
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
	cmd[1][i] = '\0';
	if (ft_strlen(cmd[1]) == 0)
	{
		free(cmd[1]);
		cmd[1] = NULL;
	}
	return (str);
}

static void	cut_flags_args(char **cmd)
{
	int		i;
	int		len;
	char	*tmp;

	i = 0;
	len = 0;
	while (cmd[1][len] && cmd[1][len] == '-')
	{
		while (cmd[1][len] && !is_space(cmd[1][len]))
			len++;
		while (is_space(cmd[1][len]))
			len++;
		if (cmd[1][len] == '-')
			continue ;
		if (!cmd[1][len])
			return ;
		else
		{
			tmp = ft_strdup(cmd[1]);
			free(cmd[1]);
			cmd[1] = (char *)malloc(sizeof(char) * (len + 1));
			while (i < len)
			{
				cmd[1][i] = tmp[i];
				i++;
			}
			cmd[1][i] = '\0';
			free(tmp);
		}
	}
}

static int	count_args()
{
	
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
		
	cmd = (char **)malloc(sizeof(char *) * 4);
	cmd[0] = cut_cmd_flags(argv, cmd);
	// if (!cmd[0])
	// 	error free etc
	cut_flags_args(cmd);
	cmd[2] = NULL;
	cmd[3] = NULL;
	printf("cmd[0]=%s\n", cmd[0]);
	printf("cmd[1]=%s\n", cmd[1]);
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