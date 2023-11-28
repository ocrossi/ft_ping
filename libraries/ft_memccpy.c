/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_memccpy.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/04 14:13:55 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/18 18:38:51 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	while (n)
	{
		if (*(unsigned char*)src == (unsigned char)c)
		{
			*(unsigned char*)dst++ = *(unsigned char*)src++;
			return (dst);
		}
		*(unsigned char*)dst++ = *(unsigned char*)src++;
		n--;
	}
	return (NULL);
}
