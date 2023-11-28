/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_lstiter.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <ocrossi@student.le-101.fr>        +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/23 18:57:06 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/23 19:16:16 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f) (t_list *elem))
{
	if (!lst)
		return ;
	if ((lst)->next)
	{
		ft_lstiter((lst->next), f);
		f(lst);
	}
	else
		f(lst);
}
