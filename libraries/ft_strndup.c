/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strndup.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/02 14:16:27 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/11/21 17:36:02 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t len)
{
	int		i;
	char	*ret;

	i = 0;
	if (!s || len == 0)
		return (NULL);
	if (!(ret = ft_strnew(len)))
		return (NULL);
	while (len && s[i])
	{
		ret[i] = s[i];
		i++;
		len--;
	}
	return (ret);
}
