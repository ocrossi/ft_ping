/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_lstprint.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/25 16:09:03 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/31 11:58:27 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstprint(t_list *lst)
{
	int		i;

	i = 1;
	if (lst == NULL)
		return ;
	while (lst)
	{
		ft_putstr(M_LINE);
		ft_putstr(M_LST1);
		ft_putnbr(lst->content_size);
		ft_putchar('\n');
		ft_putstr(M_LST2);
		ft_putnbr(i);
		ft_putchar('\n');
		i++;
		ft_putstr(M_LST3);
		ft_putstr(lst->content);
		ft_putchar('\n');
		ft_putstr(M_LINE);
		lst = lst->next;
	}
}
