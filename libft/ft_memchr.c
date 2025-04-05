/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:22:42 by zsalah            #+#    #+#             */
/*   Updated: 2024/08/29 17:07:17 by zsalah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

void	*ft_memchr(void*s, int c, size_t n)
{
	unsigned char	*str;
	size_t			i;

	i = 0;
	str = s;
	while (n > i)
	{
		if (str[i] == (unsigned char)c)
		{
			return ((void *)(str + i));
		}
		i++;
	}
	return (NULL);
}
