/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 21:57:57 by zsalah            #+#    #+#             */
/*   Updated: 2024/09/01 23:20:38 by zsalah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	len_dest;
	size_t	len_src;

	len_dest = ft_strlen(dest);
	len_src = ft_strlen(src);
	if (len_dest >= size)
	{
		len_dest = size;
		return (len_dest + len_src);
	}
	if (len_src < size - len_dest)
	{
		ft_memcpy (dest + len_dest, src, len_src + 1);
	}
	else
	{
		ft_memcpy (dest + len_dest, src, size - len_dest - 1);
		dest[size - 1] = '\0';
	}
	return (len_dest + len_src);
}
