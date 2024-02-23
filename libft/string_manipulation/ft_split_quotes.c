/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 14:49:51 by mleibeng          #+#    #+#             */
/*   Updated: 2024/02/16 21:31:03 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

const char	*handle_quotes(const char *s, char *dest, size_t *j)
{
	char	quote_char;

	quote_char = *s++;
	dest[(*j)++] = quote_char;
	while (*s)
	{
		if (*s == quote_char && *(s - 1) != '\\')
		{
			if (*(s + 1) == quote_char && *(s + 2) != '\\')
			{
				dest[(*j)++] = *s++;
				dest[(*j)++] = *s++;
				break ;
			}
			else
				break ;
		}
		else if (*s == '\\' && (*(s + 1) == quote_char || *(s + 1) == '\\'))
			dest[(*j)++] = *s++;
		dest[(*j)++] = *s++;
	}
	return (s);
}

const char	*fill_with_quotes(const char *s, char *dest, size_t *j)
{
	if ((*s == '"' || *s == '\'') && (*j == 0 || (*(s - 1) != '\\'
				&& *(s - 2) != '\\')))
		s = handle_quotes(s, dest, j);
	else
		dest[(*j)++] = *s;
	return (s);
}

size_t	fill_split_array(const char *s, char **split_arr,
		const char *delimiters, size_t string_count)
{
	size_t	i;
	size_t	j;
	size_t	s_len;

	i = 0;
	while (i < string_count && *s)
	{
		split_arr[i] = malloc((s_len = ft_substr_len(s, delimiters)) + 1);
		if (!split_arr[i])
			return (i);
		j = 0;
		while (j < s_len)
		{
			s = fill_with_quotes(s, split_arr[i], &j);
			if (*s)
				s++;
		}
		split_arr[i][j] = '\0';
		while (*s && ft_strchr(delimiters, *s))
			s++;
		i++;
	}
	return (i);
}

char	**ft_split_quotes(const char *string, const char *delimiters)
{
	char	**split_arr;
	size_t	string_count;
	size_t	i;

	string_count = split_arr_len(string, delimiters);
	split_arr = (char **)malloc((string_count + 1) * sizeof(char *));
	if (split_arr == NULL)
		return (NULL);
	while (*string && ft_strchr(delimiters, *string))
	{
		string++;
	}
	i = fill_split_array(string, split_arr, delimiters, string_count);
	split_arr[i] = NULL;
	return (split_arr);
}
