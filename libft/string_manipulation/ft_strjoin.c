/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 14:49:58 by mleibeng          #+#    #+#             */
/*   Updated: 2023/10/31 23:17:35 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strncpy(char *new_s, char const *s1, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size && s1[i])
	{
		new_s[i] = s1[i];
		i++;
	}
	return (new_s);
}

char	*ft_strncat(char *new_s, char const *s2, size_t start, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size && s2[i])
	{
		new_s[start + i] = s2[i];
		i++;
	}
	return (new_s);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*new_s;

	if (s1 == NULL)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	new_s = (char *) malloc((s1_len + s2_len + 1) * sizeof(char));
	if (new_s == NULL)
		return (NULL);
	if (new_s != NULL)
	{
		ft_strncpy(new_s, s1, s1_len);
		ft_strncat(new_s, s2, s1_len, s2_len);
	}
	new_s[s1_len + s2_len] = '\0';
	return (new_s);
}
