/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 20:49:25 by zsalah            #+#    #+#             */
/*   Updated: 2024/09/02 21:21:12 by zsalah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<stdlib.h>

void	*ft_calloc(size_t nmem, size_t size)
{
	char	*str;
	size_t	i;

	i = 0;
	str = malloc(nmem * size);
	if (str == NULL)
	{
		return (NULL);
	}
	while (i < nmem * size)
	{
		str[i] = 0;
		i++;
	}
	return (str);
}
