/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/04 12:03:19 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/24 13:45:45 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

static char	*it_works(int i, int j, const char *hs, const char *nd)
{
	const char	*tmp;
	int			k;

	tmp = &hs[i];
	k = i;
	while (hs[k] == nd[j])
	{
		if (nd[j + 1] == '\0')
			return ((char*)tmp);
		k++;
		j++;
	}
	return (NULL);
}

char		*ft_strstr(const char *hs, const char *nd)
{
	int			i;
	int			j;
	const char	*tmp;

	i = 0;
	j = 0;
	if (nd[0] == '\0')
		return ((char*)hs);
	while (hs[i])
	{
		if (hs[i] == nd[j])
		{
			tmp = it_works(i, j, hs, nd);
			if (tmp)
				return ((char*)tmp);
			j = 0;
		}
		i++;
	}
	return (NULL);
}
