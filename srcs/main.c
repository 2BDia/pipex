/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 13:05:57 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/17 16:30:07 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	init_vars(int *pid1, int *pid2, int *n)
{
	*pid1 = 0;
	*pid2 = 0;
	*n = 0;
}

int	main(int argc, char **argv, char **env)
{
	char	***cmd;
	int		n;
	t_vars	vars;
	int		pid1;
	int		pid2;

	cmd = NULL;
	if (argc < 5)
		return (errors_main(cmd, 3));
	cmd = (char ***)malloc(sizeof(char **) * argc);
	init_vars(&pid1, &pid2, &n);
	while (n < argc - 3)
	{
		cmd[n] = parse_cmd(argv, n + 2);
		if (!cmd[n])
			return (errors_main(cmd, 2));
		n++;
	}
	cmd[n++] = &argv[1];
	cmd[n++] = &argv[argc - 1];
	cmd[n] = NULL;
	vars.n = n;
	vars.env = env;
	pipex(cmd, vars, pid1, pid2);
	return (0);
}
