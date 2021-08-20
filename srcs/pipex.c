/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 14:44:22 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/20 15:52:13 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

#include "pipex.h"

static int	middle(char ***cmd, t_vars vars, int *pipein, int *pipeout)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (errors_main(cmd, 1));
	if (pid == 0)
		middle_fork(cmd, vars, pipein, pipeout);
	close(pipein[0]);
	close(pipein[1]);
	// close(pipeout[1]);
	return (1);
}

static int	loop_conditions(char ***cmd, t_vars vars, int *pipe1, int *pipe2)
{
	if (vars.check == 1)
	{
		if (pipe(pipe2) == -1)
			return (errors_main(cmd, 0));
	}
	else
	{
		if (pipe(pipe1) == -1)
			return (errors_main(cmd, 0));
	}
	if (vars.check == 1)
	{
		if (!middle(cmd, vars, pipe1, pipe2))
			return (0);
	}
	else
	{
		if (!middle(cmd, vars, pipe2, pipe1))
			return (0);
	}
	return (1);
}

static int	middle_loop(char ***cmd, t_vars vars, int *pipe1, int *pipe2)
{
	vars.i = 0;
	vars.check = 1;
	while (vars.i < vars.n - 4)
	{
		vars.i += 1;
		if (!loop_conditions(cmd, vars, pipe1, pipe2))
			return (0);
		if (vars.check == 1)
			vars.check = 2;
		else
			vars.check = 1;
		wait(0);
	}
	//you should call last fork
	return (vars.check);
}

static void	pipex_last_fork(char ***cmd, t_vars vars, int *pipen, int pid2)
{
	if (pid2 == 0)
		last(cmd, pipen, vars);
	free_close(vars.path, cmd, pipen, 1);
}

int	pipex(char ***cmd, t_vars vars, int pid1, int pid2)
{
	int		pipe1[2];
	int		pipe2[2];
	int		check;

	vars.path = split_paths(vars.env);
	if (pipe(pipe1) == -1)
		return (errors_main(cmd, 0));
	pid1 = fork();
	if (pid1 == -1)
		return (errors_main(cmd, 1));
	if (pid1 == 0)
		first(cmd, pipe1, vars);
	check = middle_loop(cmd, vars, pipe1, pipe2);
	if (!check)
		return (0);
	pid2 = fork();
	if (pid2 == -1)
		return (errors_main(cmd, 1));
	if (check == 2)
		pipex_last_fork(cmd, vars, pipe2, pid2);
	else
		pipex_last_fork(cmd, vars, pipe1, pid2);
	wait(0);
	wait(0);
	return (0);
}