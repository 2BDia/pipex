/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 13:06:17 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/12 14:59:10 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "libft.h"

/*
** cut_cmd.c
*/
char	*cut_cmd_flags(char **argv, char **cmd, int mod);
int		cut_flags_args(char **cmd);

/*
** free.c
*/
void	close_err_dup2(int fd, char **path, char ***cmd, int *pipefd);
void	free_arrays(char **cmd);
void	free_close(char **path, char ***cmd, int *pipefd, int mod);
void	exit_msg(char **path, char ***cmd, int *pipefd, int mod);

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
void	pipex(char ***cmd, char **env, int *pipefd);

/*
** utils.c
*/
int		is_space(char c);

#endif