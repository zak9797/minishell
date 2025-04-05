/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 13:51:13 by zsalah            #+#    #+#             */
/*   Updated: 2024/09/07 21:45:44 by zsalah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	word_count(const char *str, char c)
{
	size_t	count;
	int		in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (*str != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*str == c)
		{
			in_word = 0;
		}
		str++;
	}
	return (count);
}

static int	handle_error(char **token_v, size_t allocated)
{
	size_t	i;

	i = 0;
	while (i < allocated)
	{
		free(token_v[i]);
		i++;
	}
	free(token_v);
	return (1);
}

static int	fill(char **token_v, const char *s, char delimiter)
{
	size_t	len;
	size_t	i;

	len = 0;
	i = 0;
	while (*s)
	{
		while (*s == delimiter && *s)
			s++;
		len = 0;
		while (*s != delimiter && *s && ++len)
			s++;
		if (len > 0)
		{
			token_v[i] = malloc(len + 1);
			if (!token_v[i])
				return (handle_error(token_v, i));
			ft_memcpy(token_v[i], s - len, len);
			token_v[i][len] = '\0';
			i++;
		}
	}
	return (0);
}

char	**ft_split(const char *s, char c)
{
	size_t	tokens;
	char	**token_v;
	size_t	i;

	if (!s)
		return (NULL);
	tokens = word_count(s, c);
	token_v = malloc((tokens + 1) * sizeof(char *));
	if (!token_v)
		return (NULL);
	i = 0;
	while (i < tokens)
		token_v[i++] = NULL;
	token_v[tokens] = NULL;
	if (fill(token_v, s, c))
		return (NULL);
	return (token_v);
}
