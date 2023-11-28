/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strsplit.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/16 15:28:11 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/30 15:14:05 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

static char		**test_word(char **ret, int tst)
{
	if (tst == 1)
	{
		ret[0] = NULL;
		ret[1] = NULL;
		return (ret);
	}
	else
		return (ret);
}

static int		ft_lenword(char const *str, unsigned int i, char c)
{
	int j;

	j = 0;
	while (str[i])
	{
		if (str[i] == c || str[i] == '\0')
			return (j);
		i++;
		j++;
	}
	return (j);
}

static int		ft_countword(char const *str, char c)
{
	int i;
	int cpt;

	i = 0;
	cpt = 0;
	while (str[i])
	{
		if ((i == 0 && str[i] != c) || (str[i] == c &&
				(str[i + 1] != c && str[i + 1] != '\0')))
			cpt++;
		i++;
	}
	return (cpt);
}

char			**ft_strsplit(char const *s, char c)
{
	unsigned int	i;
	unsigned int	k;
	unsigned int	nw;
	int				tst;
	char			**ret;

	if (!s)
		return (NULL);
	nw = ft_countword(s, c);
	i = 0;
	k = 0;
	tst = (nw == 0) ? 1 : 0;
	if (!(ret = (char**)malloc(sizeof(char*) * (nw + tst) + 1)))
		return (NULL);
	ret[nw + tst] = 0;
	while (i < nw)
	{
		while (s[k] == c)
			k++;
		ret[i] = ft_strsub(s, k, ft_lenword(s, k, c));
		k += ft_lenword(s, k, c);
		i++;
	}
	return (test_word(ret, tst));
}
