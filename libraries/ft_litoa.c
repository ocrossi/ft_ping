/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_litoa.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/22 22:53:20 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2019/04/11 18:11:07 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_litoa(unsigned long long n)
{
	int					cpt;
	char				*ret;
	unsigned long long	nb;
	int					neg;

	neg = 0;
	cpt = ft_count_lnumber(n);
	nb = n;
	if (!(ret = ft_strnew(cpt + neg)))
		return (NULL);
	while (cpt - neg)
	{
		ret[cpt - 1] = nb % 10 + 48;
		nb /= 10;
		cpt--;
	}
	if (neg == 1)
		ret[0] = '-';
	return (ret);
}
