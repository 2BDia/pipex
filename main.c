/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 13:05:57 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/04 13:44:07 by rvan-aud         ###   ########.fr       */
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

static void	child(char **cmd1, char **path, char **env, char **argv)
{
	char	*tmp;
	int		i;
	int		fd;

	i = 0;
	fd = open(argv[1], O_RDONLY);
	if (dup2(fd, STDIN_FILENO) == 1)
		write(1, "dup2 failed\n", 12); //close free etc
	tmp = ft_strdup(cmd1[0]);
	while (execve(*cmd1, cmd1, env) == -1)
	{
		free(cmd1[0]);
		cmd1[0] = ft_strjoin(path[i], tmp);
		i++;
	}
	close(fd);
	close(0);
	free(tmp);
}

static char	**parse_cmd(char **argv, int mod)
{
	char	**cmd;
	int		n;

	cmd = (char **)malloc(sizeof(char *) * (count_args(argv[mod]) + 1));
	cmd[0] = cut_cmd_flags(argv, cmd, mod);
	// if (!cmd[0])
	// 	error free etc
	if (cmd[1])
	{
		n = cut_flags_args(cmd);
		// if (n == -1)
		// 	error free etc
		cmd[n] = NULL;
	}
	printf("cmd[0]=%s\n", cmd[0]);
	printf("cmd[1]=%s\n", cmd[1]);
	printf("cmd[2]=%s\n", cmd[2]);
	printf("cmd[3]=%s\n", cmd[3]);
	return (cmd);
}

int	main(int argc, char **argv, char **env)
{
	char		**cmd1;
	char		**cmd2;
	char		**path;

	if (argc != 5)
		return (0);
	cmd1 = parse_cmd(argv, 2);
	cmd2 = parse_cmd(argv, 3);
	path = split_paths(env);
	if (fork() == 0) //Child
	{
		child(cmd1, path, env, argv);
	}
	free_arrays(cmd1);
	free_arrays(cmd2);
	free_arrays(path);
	system("leaks pipex");
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
