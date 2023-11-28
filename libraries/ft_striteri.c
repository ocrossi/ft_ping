/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_striteri.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <ocrossi@student.le-101.fr>        +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/09 15:19:21 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/25 13:57:35 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	size_t			l;
	unsigned int	i;

	if (!(s) || !(f))
		return ;
	l = ft_strlen(s);
	i = 0;
	while (l && *s)
	{
		f(i, s);
		s++;
		i++;
		l--;
	}
}
