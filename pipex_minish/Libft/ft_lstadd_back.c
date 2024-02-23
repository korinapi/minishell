/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 15:32:02 by cpuiu             #+#    #+#             */
/*   Updated: 2023/10/07 16:09:19 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*lstone;

	if (lst)
	{
		if (*lst)
		{
			lstone = ft_lstlast(*lst);
			lstone->next = new;
		}
		else
			*lst = new;
	}
}
