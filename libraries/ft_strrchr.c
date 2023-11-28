/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strrchr.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/04 11:59:07 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/22 15:32:34 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int			cpt;

	cpt = 0;
	while (*(char*)s != '\0')
	{
		s++;
		cpt++;
	}
	while (cpt + 1)
	{
		if (*s == c)
			return ((char*)s);
		s--;
		cpt--;
	}
	return (NULL);
}
