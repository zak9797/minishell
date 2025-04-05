/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 20:49:46 by zsalah            #+#    #+#             */
/*   Updated: 2024/08/29 21:04:53 by zsalah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	uc;
	size_t			i;

	str = s;
	uc = (unsigned char )c;
	i = 0;
	while (n > i)
	{
		str[i] = uc;
		i++;
	}
	return (s);
}
