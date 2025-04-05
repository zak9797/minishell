/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 20:30:23 by zsalah            #+#    #+#             */
/*   Updated: 2024/09/07 20:31:03 by zsalah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list	*last;

	if (!alst || !new)
		return ;
	if (*alst == NULL)
	{
		*alst = new;
	}
	else
	{
		last = ft_lstlast(*alst);
		last->next = new;
	}
}
