/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 13:05:57 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/02 15:52:32 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*cut_cmd(char **argv)
{
	char	*str;
	int		len;
	int		i;

	len = 0;
	i = 0;
	while (argv[2][len] && argv[2][len] != ' ')
		len++;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (argv[2][i] && argv[2][i] != ' ')
	{
		str[i] = argv[2][i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

int	main(int argc, char **argv, char **env)
{
	// char		*path[] = {"/bin/ls", "-la", (void *)0};
	int			ret;
	char		**path;

	if (argc != 5)
		return (0);
	path = (char **)malloc(sizeof(char *) * 3);
	path[0] = cut_cmd(argv);
	printf("path[0]=%s\n", path[0]);
	// if (fork() == 0) //Child
	// {
	// 	while (execve(*path, path, env) == -1)
	// 	{
			
	// 	}
	// }
	free(path[0]);
	free(path);
	system("leaks a.out");
	return (0);
}



// int main(int argc, char **argv, char **env)
// {
// 	char *coucou[] = {"/bin/echo", "-n", "hello", (void *)0};
// 	execve(*coucou, coucou, env);
// }
// while (execve() == -1)
	//cat(chemin + commande)
//execve(char *commande, char **path, char **environnement)
//char **path = {"grep", "-n", "q"}


// /Users/rvan-aud/.brew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki