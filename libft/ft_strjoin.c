/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 21:17:32 by zsalah            #+#    #+#             */
/*   Updated: 2024/09/04 23:19:43 by zsalah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strjoin(char const *s1, char const *s2)
{
    char *str;
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int len1 = ft_strlen(s1);  // Compute length of s1
    unsigned int len2 = ft_strlen(s2);  // Compute length of s2

    str = malloc(sizeof(char) * (len1 + len2 + 1));  // Allocate memory for the combined string
    if (!str)  // Check if memory allocation succeeded
        return (NULL);

    // Copy s1 to str
    while (i < len1)
    {
        str[i] = s1[i];
        i++;
    }

    // Copy s2 to str
    while (j < len2)
    {
        str[i] = s2[j];
        i++;
        j++;
    }

    str[i] = '\0';  // Null-terminate the string
    return (str);
}