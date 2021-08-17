/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 13:05:57 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/17 15:22:38 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **env)
{
	char	***cmd;
	int		n;
	t_vars	vars;

	cmd = (char ***)malloc(sizeof(char **) * argc);
	n = 0;
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
	pipex(cmd, vars);
	// system("leaks pipex");
	return (0);
}
