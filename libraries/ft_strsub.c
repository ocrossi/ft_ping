/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strsub.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/09 15:54:52 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/15 17:33:14 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char *ret;

	if (!s)
		return (NULL);
	while (start)
	{
		s++;
		start--;
	}
	if (!(ret = (char*)malloc(sizeof(char) * len + 1)))
		return (NULL);
	ret = ft_strncpy(ret, s, len);
	ret[len] = '\0';
	return (ret);
}
