/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strlcat.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/07/09 20:28:03 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/21 21:56:18 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t i;
	size_t dest_n;
	size_t src_n;

	i = 0;
	dest_n = ft_strlen(dest);
	src_n = ft_strlen(src);
	if (size < dest_n)
		return (size + src_n);
	else
	{
		while (src[i] && (dest_n + i + 1 < size))
		{
			dest[dest_n + i] = src[i];
			i++;
		}
		dest[dest_n + i] = '\0';
	}
	return (src_n + dest_n);
}
