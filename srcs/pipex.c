/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 14:44:22 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/17 14:22:51 by rvan-aud         ###   ########.fr       */
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
	int	fd;

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

static void	last(char ***cmd, int *pipe2, char **path, t_vars vars)
{
	int	fd;

	fd = open(*cmd[vars.n - 1], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
		exit_msg(path, cmd, pipe2, 1);
	if (dup2(pipe2[0], STDIN_FILENO) == -1)
		close_err_dup2(fd, path, cmd, pipe2);
	if (dup2(fd, STDOUT_FILENO) == -1)
		close_err_dup2(fd, path, cmd, pipe2);
	free_close(path, cmd, pipe2, 0);
	close(pipe2[1]);
	close(fd);
	exec_cmd(cmd[vars.n - 3], vars.env, path);
	exit_msg(path, cmd, pipe2, 4);
}

static void	middle_fork(char ***cmd, char **path, t_vars vars, int *pipe1, int *pipe2)
{
	if (dup2(pipe1[0], STDIN_FILENO) == -1)
		close_err_dup2(-1, path, cmd, pipe2);
	if (dup2(pipe2[1], STDOUT_FILENO) == -1)
		close_err_dup2(-1, path, cmd, pipe2);
	close(pipe1[1]);
	close(pipe2[0]);
	exec_cmd(cmd[1], vars.env, path);
	exit_msg(path, cmd, pipe2, 4);
}

static int	middle(char ***cmd, t_vars vars, int *pipe1, char **path, int *pipe2)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (errors_main(cmd, 1));
	if (pid == 0)
		middle_fork(cmd, path, vars, pipe1, pipe2);
	close(pipe1[0]);
	close(pipe1[1]);
	close(pipe2[1]);
	write(2, "ok\n", 3);
	return (1);
}

int	pipex(char ***cmd, t_vars vars)
{
	int		pid1;
	int		pid2;
	int		pipe1[2];
	int		pipe2[2];
	char	**path;

	path = split_paths(vars.env);
	if (pipe(pipe1) == -1)
		return (errors_main(cmd, 0));
	pid1 = fork();
	if (pid1 == -1)
		return (errors_main(cmd, 1));
	if (pid1 == 0)
		first(cmd, pipe1, path, vars);
	if (pipe(pipe2) == -1)
		return (errors_main(cmd, 0));
	if (!middle(cmd, vars, pipe1, path, pipe2))
		return (0);
	write(2, "ok\n", 3);
	pid2 = fork();
	if (pid2 == -1)
		return (errors_main(cmd, 1));
	if (pid2 == 0)
		last(cmd, pipe2, path, vars);
	free_close(path, cmd, pipe1, 1);
	wait(0);
	wait(0);
	wait(0);
	return (0);
}
