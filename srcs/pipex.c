/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 14:44:22 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/17 16:52:23 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	close(pipeout[1]);
	return (1);
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
	vars.i = 0;
	check = 0;
	while (vars.i < vars.n - 4)
	{
		vars.i += 1;
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
			if (!middle(cmd, vars, pipe1, pipe2))
				return (0);
		}
		else
		{
			if (!middle(cmd, vars, pipe2, pipe1))
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
			last(cmd, pipe2, vars);
		free_close(vars.path, cmd, pipe2, 1);
	}
	else
	{
		if (pid2 == 0)
			last(cmd, pipe1, vars);
		free_close(vars.path, cmd, pipe1, 1);
	}
	wait(0);
	wait(0);
	return (0);
}
