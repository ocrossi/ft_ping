/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_lstmap.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <ocrossi@student.le-101.fr>        +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/23 19:51:37 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/24 11:15:25 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f) (t_list *elem))
{
	t_list *fresh;

	fresh = NULL;
	if (!lst || !f)
		return (NULL);
	fresh = f(lst);
	if (lst->next)
		fresh->next = ft_lstmap(lst->next, f);
	return (fresh);
}
