/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 14:44:22 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/17 15:10:01 by rvan-aud         ###   ########.fr       */
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

static void	first(char ***cmd, int *pipe1, char **path, t_vars vars)
{
	int	fd;

	fd = open(*cmd[vars.n - 2], O_RDONLY);
	if (fd == -1)
		exit_msg(path, cmd, pipe1, 2);
	if (dup2(fd, STDIN_FILENO) == -1)
		close_err_dup2(fd, path, cmd, pipe1);
	if (dup2(pipe1[1], STDOUT_FILENO) == -1)
		close_err_dup2(fd, path, cmd, pipe1);
	close(fd);
	close(pipe1[0]);
	exec_cmd(cmd[0], vars.env, path);
	exit_msg(path, cmd, pipe1, 4);
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

static void	middle_fork(char ***cmd, char **path, t_vars vars, int *pipein, int *pipeout, int i)
{
	if (dup2(pipein[0], STDIN_FILENO) == -1)
		close_err_dup2(-1, path, cmd, pipeout);
	if (dup2(pipeout[1], STDOUT_FILENO) == -1)
		close_err_dup2(-1, path, cmd, pipeout);
	close(pipein[1]);
	close(pipeout[0]);
	exec_cmd(cmd[i], vars.env, path);
	exit_msg(path, cmd, pipeout, 4);
}

static int	middle(char ***cmd, t_vars vars, int *pipein, char **path, int *pipeout, int i)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (errors_main(cmd, 1));
	if (pid == 0)
		middle_fork(cmd, path, vars, pipein, pipeout, i);
	close(pipein[0]);
	close(pipein[1]);
	close(pipeout[1]);
	return (1);
}

int	pipex(char ***cmd, t_vars vars)
{
	int		pid1;
	int		pid2;
	int		pipe1[2];
	int		pipe2[2];
	char	**path;
	int		i;
	int		check;

	path = split_paths(vars.env);
	if (pipe(pipe1) == -1)
		return (errors_main(cmd, 0));
	pid1 = fork();
	if (pid1 == -1)
		return (errors_main(cmd, 1));
	if (pid1 == 0)
		first(cmd, pipe1, path, vars);
	i = 0;
	check = 0;
	while (i < vars.n - 4)
	{
		i += 1;
		if (check == 0)
		{
			if (pipe(pipe2) == -1)
				return (errors_main(cmd, 0));
		}
		else
		{
			if (pipe(pipe1) == -1)
				return (errors_main(cmd, 0));
		}
		if (check == 0)
		{
			if (!middle(cmd, vars, pipe1, path, pipe2, i))
				return (0);
		}
		else
		{
			if (!middle(cmd, vars, pipe2, path, pipe1, i))
				return (0);
		}
		if (check == 0)
			check = 1;
		else
			check = 0;
		wait(0);
	}
	pid2 = fork();
	if (pid2 == -1)
		return (errors_main(cmd, 1));
	if (check == 1)
	{
		if (pid2 == 0)
			last(cmd, pipe2, path, vars);
		free_close(path, cmd, pipe2, 1);
	}
	else
	{
		if (pid2 == 0)
			last(cmd, pipe1, path, vars);
		free_close(path, cmd, pipe1, 1);
	}
	wait(0);
	wait(0);
	return (0);
}
