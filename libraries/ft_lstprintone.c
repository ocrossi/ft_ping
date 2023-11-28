/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_lstprintone.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/31 11:52:51 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/31 11:59:12 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstprintone(t_list *elem)
{
	ft_putstr(M_LINE);
	ft_putstr(M_LST1);
	ft_putnbr(elem->content_size);
	ft_putchar('\n');
	ft_putstr(M_LST3);
	ft_putstr(elem->content);
	ft_putchar('\n');
	ft_putstr(M_LINE);
}
