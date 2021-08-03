/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 13:06:17 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/03 14:17:31 by rvan-aud         ###   ########.fr       */
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

void	free_arrays(char **cmd);

char	**split_paths(char **env);

int		is_space(char c);

#endif