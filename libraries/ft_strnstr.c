/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strnstr.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/04 12:09:31 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/25 14:12:44 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

static char	*does_it_work(const char *hs, const char *nd, size_t len, int i)
{
	size_t		j;
	size_t		k;
	const char	*tmp;

	tmp = &hs[i];
	j = 0;
	k = i;
	while (hs[k] == nd[j] && k < len)
	{
		if (nd[j + 1] == '\0')
			return ((char*)tmp);
		k++;
		j++;
	}
	return (NULL);
}

char		*ft_strnstr(const char *hs, const char *nd, size_t len)
{
	size_t		i;
	size_t		j;

	i = 0;
	j = 0;
	if (nd[0] == '\0')
		return ((char*)hs);
	while (hs[i] && i <= len)
	{
		if (hs[i] == nd[j])
		{
			if (does_it_work(hs, nd, len, i))
				return (does_it_work(hs, nd, len, i));
		}
		i++;
	}
	return (NULL);
}
