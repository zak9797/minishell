/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 23:30:55 by zsalah            #+#    #+#             */
/*   Updated: 2024/09/04 23:40:03 by zsalah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strndup(const char *s, size_t n)
{
    size_t i = 0;
    char *dup = malloc(n + 1); // +1 for null terminator

    if (!dup)
        return NULL;

    while (i < n && s[i])
    {
        dup[i] = s[i];
        i++;
    }

    dup[i] = '\0'; // Always null-terminate
    return dup;
}