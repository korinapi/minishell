/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:32:01 by cpuiu             #+#    #+#             */
/*   Updated: 2023/10/06 20:57:55 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*new_str;
	size_t	s1_len;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (set[0] == '\0' || s1[0] == '\0')
		return (ft_strdup(s1));
	s1_len = ft_strlen(s1);
	while (ft_strchr(set, s1[i]))
		i++;
	while (ft_strrchr(set, s1[s1_len - j - 1]))
		j++;
	new_str = ft_substr(s1, i, s1_len - i - j);
	return (new_str);
}
