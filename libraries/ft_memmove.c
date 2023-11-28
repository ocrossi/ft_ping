/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_memmove.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/04 15:44:29 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/25 14:14:28 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*d;
	const char	*s;

	d = (char*)dst;
	s = (const char*)src;
	if (dst < src)
		d = ft_memcpy(dst, src, len);
	else
	{
		if (len)
		{
			while (--len)
			{
				d[len] = s[len];
			}
			d[len] = s[len];
		}
	}
	return (dst);
}
