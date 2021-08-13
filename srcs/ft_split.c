/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 14:46:09 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/08/13 14:46:14 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	freestrs(char **strs, int j)
{
	while (j < 0)
		free(strs[--j]);
	free(strs);
	return (0);
}

static int	countwords(char const *s, char c)
{
	int	wc;
	int	i;

	wc = 0;
	i = 0;
	while (s[i])
	{
		if ((s[i + 1] == c || s[i + 1] == '\0') && s[i] != c)
			wc++;
		i++;
	}
	return (wc);
}

static int	countnext(char const *s, char c, int i)
{
	int	wl;

	wl = 0;
	while (s[i] && s[i] == c)
		i++;
	while (s[i] && s[i] != c)
	{
		i++;
		wl++;
	}
	return (wl);
}

static int	alnput(char const *s, char **strs, char c, int wc)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	while (s[i] && j < wc)
	{
		strs[j] = (char *)malloc(sizeof(char) * (countnext(s, c, i) + 1));
		if (!strs[j])
			return (freestrs(strs, j));
		while (s[i] && s[i] == c)
			i++;
		k = 0;
		while (s[i] && s[i] != c)
			strs[j][k++] = s[i++];
		strs[j][k] = '\0';
		j++;
	}
	strs[j] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	int		wc;

	if (!s)
		return (NULL);
	wc = countwords(s, c);
	strs = (char **)malloc(sizeof(char *) * (wc + 1));
	if (!strs)
		return (NULL);
	if (wc == 0)
		strs[0] = NULL;
	else if (alnput(s, strs, c, wc) == 0)
		return (NULL);
	return (strs);
}
