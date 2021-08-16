/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 16:30:25 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/16 14:51:22 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_err_dup2(int fd, char **path, char ***cmd, int *pipefd)
{
	if (fd != -1)
		close(fd);
	exit_msg(path, cmd, pipefd, 3);
}

void	free_arrays(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
		free(cmd[i++]);
	free(cmd);
}

void	free_close(char **path, char ***cmd, int *pipefd, int mod)
{
	int	i;

	i = 0;
	if (mod == 1)
	{
		if (path)
			free_arrays(path);
		while (cmd[i - 1])
		{
			printf("cmd[%d]=%s\n", i, *cmd[i]);
			free_arrays(cmd[i++]);
		}
	}
	close(pipefd[0]);
	close(pipefd[1]);
}

void	exit_msg(char **path, char ***cmd, int *pipefd, int mod)
{
	free_close(path, cmd, pipefd, 1);
	if (mod == 0)
		write(1, "Error : fork failed\n", 20);
	else if (mod == 1)
		write(1, "Error : outfile opening failed\n", 30);
	else if (mod == 2)
		write(1, "Error : infile opening failed\n", 30);
	else if (mod == 3)
		write(1, "Error : dup2 failed\n", 20);
	else if (mod == 4)
	{
		write(2, "Error : command not found\n", 26);
		exit(-1);
	}
	exit(0);
}
