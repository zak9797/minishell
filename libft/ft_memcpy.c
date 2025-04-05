/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memcpy.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 19:46:43 by zsalah            #+#    #+#             */
/*   Updated: 2024/08/30 21:19:38 by zsalah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*dest1;
	const unsigned char	*src1;
	size_t				i;

	dest1 = dest;
	src1 = src;
	i = 0;
	if (dest == NULL && src == NULL)
	{
		return (NULL);
	}
	while (n--)
	{
		dest1[i] = src1[i];
		i++;
	}
	return (dest);
}
