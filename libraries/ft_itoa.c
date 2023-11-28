/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_itoa.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <ocrossi@student.le-101.fr>        +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/10 15:43:58 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/24 11:56:37 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n)
{
	int		cpt;
	char	*ret;
	long	nb;
	int		neg;

	neg = 0;
	cpt = ft_count_integer(n);
	nb = (long)n;
	if (nb < 0)
	{
		neg = 1;
		cpt++;
		nb = -nb;
	}
	if (!(ret = ft_strnew(cpt)))
		return (NULL);
	while (cpt)
	{
		ret[cpt - 1] = nb % 10 + 48;
		nb /= 10;
		cpt--;
	}
	if (neg == 1)
		ret[0] = '-';
	return (ret);
}
