/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 13:05:57 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/16 15:02:03 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	errors_main(char ***cmd, int mod)
{
	if (mod == 2)
		free(cmd[0]);
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
	char	***cmd;
	int		pid;
	int		pipefd[2];
	int		n;
	t_vars	vars;

	cmd = (char ***)malloc(sizeof(char **) * argc);
	n = 0;
	while (n < argc - 3)
	{
		cmd[n] = parse_cmd(argv, n + 2);
		// printf("cmd[%d][0]=%s\n", n, cmd[n][0]);
		// printf("cmd[%d][0]=%s\n", n, cmd[n][1]);
		if (!cmd[n])
			return (errors_main(cmd, 2));
		n++;
	}
	cmd[n++] = &argv[1];
	cmd[n++] = &argv[argc - 1];
	cmd[n] = NULL;
	vars.n = n;
	vars.env = env;
	if (pipe(pipefd) == -1)
		return (errors_main(cmd, 0));
	pid = fork();
	if (pid == -1)
		return (errors_main(cmd, 1));
	if (pid == 0)
		pipex(cmd, pipefd, vars);
	write(1, "ok\n", 3);
	free_close(NULL, cmd, pipefd, 1);
	system("leaks pipex");
	return (0);
}
