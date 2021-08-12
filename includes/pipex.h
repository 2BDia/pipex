/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 13:06:17 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/12 14:47:23 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "libft.h"

# include <stdio.h> //to remove

// typedef struct s_params
// {
// 	char	*infile;
// 	char	*outfile;
// 	char	*cmd1;
// 	char	*cmd2;
// }	t_params;

void	close_err_dup2(int fd, char **path, char ***cmd, int *pipefd);
void	free_arrays(char **cmd);
void	free_close(char **path, char ***cmd, int *pipefd, int mod);
void	exit_msg(char **path, char ***cmd, int *pipefd, int mod);

char	**split_paths(char **env);

int		is_space(char c);

char	**parse_cmd(char **argv, int mod);

char	*cut_cmd_flags(char **argv, char **cmd, int mod);
int		cut_flags_args(char **cmd);

void	pipex(char ***cmd, char **env, int *pipefd);

#endif