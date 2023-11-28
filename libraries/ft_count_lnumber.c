/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_count_integer.c                               .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/15 15:10:27 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2019/04/11 18:10:42 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_count_lnumber(unsigned long long n)
{
	int cpt;

	cpt = 0;
	if (n == 0)
		cpt = 1;
	while (n != 0)
	{
		n = n / 10;
		cpt++;
	}
	return (cpt);
}
