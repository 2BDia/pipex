/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 13:05:57 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/12 14:55:18 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	errors_main(char ***cmd, int mod)
{
	if (mod == 2)
		free(cmd[0]);
	else if (mod == 3)
		write(1, "Error : arguments count not valid\n", 34);
	else
	{
		free_arrays(cmd[0]);
		free_arrays(cmd[1]);
		if (mod == 0)
			write(1, "Error : pipe failed\n", 20);
		else if (mod == 1)
			write(1, "Error : fork failed\n", 20);
	}
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char		**cmd[4];
	int			pid;
	int			pipefd[2];

	if (argc != 5)
		return (errors_main(cmd, 3));
	cmd[0] = parse_cmd(argv, 2);
	if (!cmd[0])
		return (0);
	cmd[1] = parse_cmd(argv, 3);
	if (!cmd[1])
		return (errors_main(cmd, 2));
	cmd[2] = &argv[1];
	cmd[3] = &argv[4];
	if (pipe(pipefd) == -1)
		return (errors_main(cmd, 0));
	pid = fork();
	if (pid == -1)
		return (errors_main(cmd, 1));
	if (pid == 0)
		pipex(cmd, env, pipefd);
	free_close(NULL, cmd, pipefd, 1);
	return (0);
}
