/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 13:05:57 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/11 15:13:05 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		if (!*cmd)
		{
			free(cmd);
			break ;
		}
		i++;
	}
}

static void	child(char ***cmd, char **env, int *pipefd, char **path)
{
	int		fd;

	fd = open(*cmd[2], O_RDONLY);
	if (fd == -1)
		exit_msg(path, cmd, pipefd, 2);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		exit_msg(path, cmd, pipefd, 3);
	}
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		close(fd);
		exit_msg(path, cmd, pipefd, 3);
	}
	close(fd);
	exec_cmd(cmd[0], env, path);
	exit_msg(path, cmd, pipefd, 4);
}

static void	pipex(char ***cmd, char **env, int *pipefd)
{
	int		pid;
	int		fd;
	char	**path;

	path = split_paths(env);
	pid = fork();
	if (pid == -1)
		exit_msg(path, cmd, pipefd, 0);
	if (pid == 0)
		child(cmd, env, pipefd, path);
	wait(0);
	fd = open(*cmd[3], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
		exit_msg(path, cmd, pipefd, 1);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		close(fd);
		exit_msg(path, cmd, pipefd, 3);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		exit_msg(path, cmd, pipefd, 3);
	}
	free_close(path, cmd, pipefd, 0);
	close(fd);
	exec_cmd(cmd[1], env, path);
	exit_msg(path, cmd, pipefd, 4);
}

int	main(int argc, char **argv, char **env)
{
	char		**cmd[4];
	int			pid;
	int			pipefd[2];

	if (argc != 5)
	{
		write(1, "Error : arguments count not valid\n", 34);
		return (0);
	}
	cmd[0] = parse_cmd(argv, 2);
	if (!cmd[0])
		return (0);
	cmd[1] = parse_cmd(argv, 3);
	if (!cmd[1])
	{
		free(cmd[0]);
		return (0);
	}
	cmd[2] = &argv[1];
	cmd[3] = &argv[4];
	if (pipe(pipefd) == -1)
	{
		free_arrays(cmd[0]);
		free_arrays(cmd[1]);
		write(1, "Error : pipe failed\n", 20);
		return (0);
	}
	pid = fork();
	if (pid == -1)
	{
		free_arrays(cmd[0]);
		free_arrays(cmd[1]);
		write(1, "Error : fork failed\n", 20);
		return (0);
	}
	if (pid == 0)
		pipex(cmd, env, pipefd);
	free_arrays(cmd[0]);
	free_arrays(cmd[1]);
	close(pipefd[0]);
	close(pipefd[1]);
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
