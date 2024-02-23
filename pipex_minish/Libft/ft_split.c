/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:59:41 by cpuiu             #+#    #+#             */
/*   Updated: 2024/01/31 14:21:51 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_new_array(char **new_array)
{
	int	i;

	i = 0;
	while (new_array[i])
		free(new_array[i++]);
	free(new_array);
}

char	*fill_new_array(char *new_array, char const *s, int len, int word_len)
{
	int		i;

	i = 0;
	while (word_len > 0)
	{
		new_array[i] = s[len - word_len];
		i++;
		word_len--;
	}
	new_array[i] = '\0';
	return (new_array);
}

char	**each_word(char const *s, char c, int words, int word_len)
{
	char	**new_array;
	int		len;
	int		index;

	new_array = (char **)malloc(sizeof(char *) * (words + 1));
	if (!new_array)
		return (NULL);
	len = 0;
	index = 0;
	while (index < words)
	{
		word_len = 0;
		while (s[len] && s[len] == c)
			len++;
		while (s[len] && s[len] != c && ++word_len)
			len++;
		new_array[index] = (char *)malloc(sizeof(char) * (word_len + 1));
		if (!new_array[index])
			return (free_new_array(new_array), NULL);
		fill_new_array(new_array[index], s, len, word_len);
		index++;
	}
	new_array[index] = 0;
	return (new_array);
}

char	**ft_split(char const *s, char c)
{
	char	**new_array;
	int		words;
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	words = 0;
	while (s[i])
	{
		if ((i == 0 || s[i - 1] == c) && s[i] != c)
			words++;
		i++;
	}
	new_array = each_word(s, c, words, 0);
	return (new_array);
}
