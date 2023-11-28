/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_memalloc.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/15 17:20:29 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/15 17:25:16 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	int *r;
	int *tmp;

	if (!(r = (int*)malloc(sizeof(int) * size)))
		return (NULL);
	tmp = r;
	while (size)
	{
		*r = 0;
		r++;
		size--;
	}
	return (tmp);
}
