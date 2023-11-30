/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   print_memory.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: kgrosjea <kgrosjea@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/24 13:53:51 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2019/12/03 12:30:36 by kgrosjea    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	set_octet(unsigned char octet[], unsigned char *p)
{
	if (*p & 0x80)
		octet[0]++;
	if (*p & 0x40)
		octet[1]++;
	if (*p & 0x20)
		octet[2]++;
	if (*p & 0x10)
		octet[3]++;
	if (*p & 0x8)
		octet[4]++;
	if (*p & 0x4)
		octet[5]++;
	if (*p & 0x2)
		octet[6]++;
	if (*p & 0x1)
		octet[7]++;
}

void	print_octet(unsigned char *p)
{
	unsigned char	octet[8];
	int				i;
	char			sp;

	sp = ' ';
	ft_memset(&octet, 48, sizeof(octet));
	set_octet(octet, p);
	i = 0;
	while (i < 8)
	{
		if (i == 4)
			write(1, &sp, 1);
		write(1, &octet[i], 1);
		i++;
	}
}

void	print_memory_binary(const void *addr, size_t size)
{
	unsigned char	*p;
	int				i;

	p = (unsigned char *)addr;
	i = 0;
	while (size)
	{
		if (i == 4)
		{
			ft_putstr(" * \n");
			i = 0;
			continue ;
		}
		print_octet(p);
		ft_putstr(" || ");
		p++;
		i++;
		size--;
	}
}

void	print_memory(const void *addr, size_t size, int base)
{
	unsigned char	*new;

	if (!(new = (unsigned char *)malloc(sizeof(new) * size)))
		return ;
	ft_memcpy(new, addr, size);
	if (base == 2)
		print_memory_binary(new, size);
	else if (base == 16)
		print_memory_hexa(new, size);
	else
		write(1, "Wrong base\n", 11);
	write(1, "\n", 1);
	free(new);
}
