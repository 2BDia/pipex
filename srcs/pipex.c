/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 14:44:22 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/16 17:24:30 by rvan-aud         ###   ########.fr       */
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

// static void	middle(char ***cmd, int *pipefd, char **path, t_vars vars, int *pipefd_2)
// {
// 	if (dup2(pipefd[0], STDIN_FILENO) == -1)
// 		write(2, "error dup2\n", 11);
// 	if (dup2(pipefd_2[1], STDOUT_FILENO) == -1)
// 		write(2, "error dup2\n", 11);
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// 	exec_cmd(cmd[1], vars.env, path);
// }

// static void	prep_middle(char ***cmd, int *pipefd, char **path, t_vars vars)
// {
// 	int	pipefd_2[2];
// 	int	pid;
// 	char	buffer[80];

// 	if (pipe(pipefd_2) == -1)
// 		write(2, "error pipe\n", 11);
// 	pid = fork();
// 	if (pid == -1)
// 		write(2, "error fork\n", 11);
// 	if (pid == 0)
// 		middle(cmd, pipefd, path, vars, pipefd_2);
// 	write(2, "ok2\n", 4);
// 	read(pipefd_2[0], buffer, 80);
// 	printf("buffer=%s\n", buffer);
// }

static void	first(char ***cmd, int *pipefd, char **path, t_vars vars)
{
	int		fd;

	fd = open(*cmd[vars.n - 2], O_RDONLY);
	if (fd == -1)
		exit_msg(path, cmd, pipefd, 2);
	if (dup2(fd, STDIN_FILENO) == -1)
		close_err_dup2(fd, path, cmd, pipefd);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		close_err_dup2(fd, path, cmd, pipefd);
	close(fd);
	close(pipefd[0]);
	exec_cmd(cmd[0], vars.env, path);
	exit_msg(path, cmd, pipefd, 4);
}

void	pipex(char ***cmd, int *pipefd, t_vars vars)
{
	int		pid1;
	int		pid2;
	int		fd;
	char	**path;
	// int		i;

	path = split_paths(vars.env);
	pid1 = fork();
	if (pid1 == -1)
		exit_msg(path, cmd, pipefd, 0);
	if (pid1 == 0)
		first(cmd, pipefd, path, vars);
	wait(0);
	// write(1, "ok1\n", 4);
	// prep_middle(cmd, pipefd, path, vars);
	// i = 0;
	// while (i < vars.n - 3)
	// {
	// 	pid = fork();
	// 	if (pid == -1)
	// 		exit_msg(path, cmd, pipefd, 0);
	// 	if (pid == 0)
	// 		middle(cmd, pipefd, path, vars);
	// 	i++;
	// }
	fd = open(*cmd[vars.n - 1], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
		exit_msg(path, cmd, pipefd, 1);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		close_err_dup2(fd, path, cmd, pipefd);
	if (dup2(fd, STDOUT_FILENO) == -1)
		close_err_dup2(fd, path, cmd, pipefd);
	free_close(path, cmd, pipefd, 0);
	close(fd);
	exec_cmd(cmd[vars.n - 3], vars.env, path);
	exit_msg(path, cmd, pipefd, 4);
}
