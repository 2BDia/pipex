/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 13:06:17 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/20 15:11:34 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

#include <stdio.h>

typedef struct s_vars
{
	int		n;
	int		i;
	int		check;
	char	**env;
	char	**path;
}	t_vars;

/*
** cut_cmd.c
*/
char	*cut_cmd_flags(char **argv, char **cmd, int mod);
int		cut_flags_args(char **cmd);

/*
** fork_functs.c
*/
void	first(char ***cmd, int *pipe1, t_vars vars);
void	last(char ***cmd, int *pipe2, t_vars vars);
void	middle_fork(char ***cmd, t_vars vars, int *pipein, int *pipeout);

/*
** free.c
*/
int		errors_main(char ***cmd, int mod);
void	close_err_dup2(int fd, char **path, char ***cmd, int *pipefd);
void	free_arrays(char **cmd);
void	free_close(char **path, char ***cmd, int *pipefd, int mod);
void	exit_msg(char **path, char ***cmd, int *pipefd, int mod);

/*
** ft_split.c
*/
char	**ft_split(char const *s, char c);

/*
** parse_cmd.c
*/
char	**parse_cmd(char **argv, int mod);

/*
** path_handling.c
*/
char	**split_paths(char **env);

/*
** pipex.c
*/
int		pipex(char ***cmd, t_vars vars, int pid1, int pid2);

/*
** utils.c
*/
int		is_space(char c);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

#endif