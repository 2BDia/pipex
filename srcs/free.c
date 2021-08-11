/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 16:30:25 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/11 13:56:05 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	if (mod == 1)
	{
		free_arrays(path);
		free_arrays(cmd[0]);
		free_arrays(cmd[1]);
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
		write(1, "Error : file 2 opening failed\n", 30);
	else if (mod == 2)
		write(1, "Error : file 1 opening failed\n", 30);
	else if (mod == 3)
		write(1, "Error : dup2 failed\n", 20);
	else if (mod == 4)
	{
		write(2, "Error : command not found\n", 26);
		exit(-1);
	}
	exit(0);
}
