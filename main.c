/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 13:05:57 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/10 15:51:58 by rvan-aud         ###   ########.fr       */
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
	// printf("cmd[0]=%s\n", cmd[0]);
	// printf("cmd[1]=%s\n", cmd[1]);
	// printf("cmd[2]=%s\n", cmd[2]);
	// printf("cmd[3]=%s\n", cmd[3]);
	return (cmd);
}

static void	exec_cmd(char **cmd, char **env, char **path)
{
	char	*tmp;
	int		i;

	tmp = ft_strdup(*cmd);
	i = 0;
	while (execve(*cmd, cmd, env) == -1 && path[i])
	{
		free(*cmd);
		*cmd = ft_strjoin(path[i], tmp);
		i++;
	}
	printf("execve error\n");
	exit (0);
}

static void	child(char ***cmd, char **env, int *pipefd, char **path)
{
	int		fd;
	(void)pipefd;

	fd = open(*cmd[2], O_RDONLY);
	//cas où fd==-1
	if (dup2(fd, STDIN_FILENO) == -1)
		write(1, "dup2 failed\n", 12); //close free etc
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		write(1, "dup2 failed\n", 12); //close free etc
	exec_cmd(cmd[0], env, path);
}

static void	pipex(char ***cmd, char **env, int *pipefd)
{
	int		pid;
	int		fd;
	char	**path;

	
	path = split_paths(env);
	pid = fork();
	if (pid == -1)
		write(1, "fork failed\n", 12); //close free etc
	if (pid == 0)
		child(cmd, env, pipefd, path);
	wait(0);
	fd = open(*cmd[3], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		write(1, "dup2 failed\n", 12); //close free etc
	if (dup2(fd, STDOUT_FILENO) == -1)
		write(1, "dup2 failed\n", 12); //close free etc
	close(pipefd[0]);
	close(pipefd[1]);
	close(fd);
	exec_cmd(cmd[1], env, path);
	free_arrays(path);
}

int	main(int argc, char **argv, char **env)
{
	char		**cmd[4];
	int			pid;
	int			pipefd[2];

	if (argc != 5)
		return (0);
	cmd[0] = parse_cmd(argv, 2);
	cmd[1] = parse_cmd(argv, 3);
	cmd[2] = &argv[1];
	cmd[3] = &argv[4];
	if (pipe(pipefd) == -1)
		write(1, "pipe failed\n", 12); //close free etc
	pid = fork();
	if (pid == -1)
		write(1, "fork failed\n", 12); //close free etc
	if (pid == 0)
		pipex(cmd, env, pipefd);
	// waitpid(0);
	free_arrays(cmd[0]);
	free_arrays(cmd[1]);
	system("leaks pipex");
	return (0);
}




// static void	child(char **cmd1, char **env, char **argv, int *pipefd)
// {
// 	char	*tmp;
// 	int		i;
// 	int		fd;
// 	char	**path;

// 	i = 0;
// 	path = split_paths(env);
// 	fd = open(argv[1], O_RDONLY);
// 	if (dup2(fd, STDIN_FILENO) == -1)
// 		write(1, "dup2 failed\n", 12); //close free etc
// 	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
// 		write(1, "dup2 failed\n", 12); //close free etc
// 	close(fd);
// 	tmp = ft_strdup(cmd1[0]);
// 	while (execve(*cmd1, cmd1, env) == -1)
// 	{
// 		free(cmd1[0]);
// 		cmd1[0] = ft_strjoin(path[i], tmp);
// 		i++;
// 	}
// 	close(0);
// 	free(tmp);
// 	free_arrays(path);
// }

// static void	piped(char **cmd2, char **env, char **argv, int *pipefd)
// {
// 	char	*tmp;
// 	int		i;
// 	int		fd;
// 	char	**path;

// 	i = 0;
// 	path = split_paths(env);
// 	fd = open(argv[4], O_RDWR | O_CREAT | O_TRUNC);
// 	printf("fd=%d\n", fd);
// 	if (dup2(pipefd[0], STDIN_FILENO) == -1)
// 		write(1, "dup2 failed\n", 12); //close free etc
// 	if (dup2(fd, STDOUT_FILENO) == -1)
// 		write(1, "dup2 failed\n", 12); //close free etc
// 	// close(fd);
// 	tmp = ft_strdup(cmd2[0]);
// 	while (execve(*cmd2, cmd2, env) == -1)
// 	{
// 		printf("cmd2=%s\n", *cmd2);
// 		free(cmd2[0]);
// 		cmd2[0] = ft_strjoin(path[i], tmp);
// 		i++;
// 	}
// 	write(1, "no\n", 3);
// 	close(0);
// 	free(tmp);
// 	free_arrays(path);
// }

// int	main(int argc, char **argv, char **env)
// {
// 	char		**cmd1;
// 	char		**cmd2;
// 	int			pid;
// 	int			pipefd[2];
// 	// char	buffer[80];

// 	if (argc != 5)
// 		return (0);
// 	cmd1 = parse_cmd(argv, 2);
// 	cmd2 = parse_cmd(argv, 3);
// 	if (pipe(pipefd) == -1)
// 		write(1, "pipe failed\n", 12); //close free etc
// 	pid = fork();
// 	if (pid == -1)
// 		write(1, "fork failed\n", 12); //close free etc
// 	if (pid == 0) //Child
// 	{
// 		child(cmd1, env, argv, pipefd);
// 	}
// 	pid = fork();
// 	if (pid == -1)
// 		write(1, "fork failed\n", 12); //close free etc
// 	if (pid == 0) //Child
// 	{
// 		piped(cmd2, env, argv, pipefd);
// 	}
// 	// read(pipefd[0], buffer, 80);
// 	// printf("buffer=%s\n", buffer);
// 	free_arrays(cmd1);
// 	free_arrays(cmd2);
// 	// system("leaks pipex");
// 	return (0);
// }










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
