/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strtrim.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <ocrossi@student.le-101.fr>        +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/09 16:44:21 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/25 14:15:19 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

static int		ft_check_trim_back(char const *s, int len, int i)
{
	int cpt;
	int j;

	i = 0;
	cpt = 0;
	while (s[i])
	{
		if (s[i] == '\t' || s[i] == '\n' || s[i] == ' ')
		{
			j = i;
			while (s[j] == '\t' || s[j] == '\n' || s[j] == ' ')
			{
				if (s[j + 1] == '\0')
					return (len - (j - i + 1));
				j++;
			}
			cpt = 0;
		}
		i++;
		cpt++;
	}
	return (len);
}

static int		ft_check_trim_front(char const *s)
{
	int len;
	int i;

	i = 0;
	len = ft_strlen(s);
	while (s[i] == '\t' || s[i] == '\n' || s[i] == ' ' || s[i] == '\0')
	{
		if (s[i] == '\0')
			return (0);
		i++;
		len--;
	}
	len = ft_check_trim_back(s, len, i);
	return (len);
}

char			*ft_strtrim(char const *s)
{
	int				len;
	unsigned int	cpt;
	char			*r;

	if (!s)
		return (NULL);
	cpt = 0;
	len = ft_check_trim_front(s);
	while (s[cpt] == '\t' || s[cpt] == '\n' || s[cpt] == ' ' || s[cpt] == '\0')
		cpt++;
	r = ft_strsub(s, cpt, len);
	return (r);
}
