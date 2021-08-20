/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_functs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:32:24 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/20 15:39:14 by rvan-aud         ###   ########.fr       */
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

void	first(char ***cmd, int *pipe1, t_vars vars)
{
	int	fd;

	// printf("pipe1=%d\n", pipe1[0]);
	fd = open(*cmd[vars.n - 2], O_RDONLY);
	if (fd == -1)
		exit_msg(vars.path, cmd, pipe1, 2);
	if (dup2(fd, STDIN_FILENO) == -1)
		close_err_dup2(fd, vars.path, cmd, pipe1);
	if (dup2(pipe1[1], STDOUT_FILENO) == -1)
		close_err_dup2(fd, vars.path, cmd, pipe1);
	close(fd);
	close(pipe1[0]);
	exec_cmd(cmd[0], vars.env, vars.path);
	exit_msg(vars.path, cmd, pipe1, 4);
}

void	last(char ***cmd, int *pipe2, t_vars vars)
{
	int	fd;

	// printf("pipe2=%d\n", pipe2[0]);
	fd = open(*cmd[vars.n - 1], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
		exit_msg(vars.path, cmd, pipe2, 1);
	if (dup2(pipe2[0], STDIN_FILENO) == -1)
	{
		write(2, "ok\n", 3);
		close_err_dup2(fd, vars.path, cmd, pipe2);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		close_err_dup2(fd, vars.path, cmd, pipe2);
	free_close(vars.path, cmd, pipe2, 0);
	close(pipe2[1]);
	close(fd);
	exec_cmd(cmd[vars.n - 3], vars.env, vars.path);
	exit_msg(vars.path, cmd, pipe2, 4);
}

void	middle_fork(char ***cmd, t_vars vars, int *pipein, int *pipeout)
{
	// printf("pipein=%d\n", pipein[0]);
	if (dup2(pipein[0], STDIN_FILENO) == -1)
	{
		write(2, "oki\n", 4);
		close_err_dup2(-1, vars.path, cmd, pipeout);
	}
	if (dup2(pipeout[1], STDOUT_FILENO) == -1)
	{
		write(2, "oko\n", 4);
		close_err_dup2(-1, vars.path, cmd, pipeout);
	}
	close(pipein[1]);
	close(pipeout[0]);
	exec_cmd(cmd[vars.i], vars.env, vars.path);
	exit_msg(vars.path, cmd, pipeout, 4);
}
