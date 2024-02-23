/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 15:44:19 by cpuiu             #+#    #+#             */
/*   Updated: 2023/10/07 16:10:46 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*i;

	if (!lst)
		return ;
	while (*lst)
	{
		i = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = i;
	}
	*lst = NULL;
}
