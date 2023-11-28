/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_memchr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/04 15:52:36 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/25 13:53:05 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char c1;
	unsigned char *s1;

	c1 = (unsigned char)c;
	s1 = (unsigned char*)s;
	while (n)
	{
		if (*s1 == c1)
			return (s1);
		s1++;
		n--;
	}
	return (NULL);
}
