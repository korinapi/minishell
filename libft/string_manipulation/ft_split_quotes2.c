/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 21:18:08 by mleibeng          #+#    #+#             */
/*   Updated: 2024/02/16 21:27:02 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_substr_len(const char *str, const char *c)
{
	size_t	count_len;

	count_len = 0;
	while (*str && !strchr(c, *str))
	{
		count_len++;
		str++;
	}
	return (count_len);
}

void	export_stuff(size_t *string_count, bool *inside_quotes)
{
	(*string_count)++;
	if (*inside_quotes == true)
		*inside_quotes = false;
	else
		*inside_quotes = true;
}

size_t	split_arr_len(const char *s, const char *c)
{
	size_t	string_count;
	bool	inside_quotes;

	string_count = 0;
	inside_quotes = false;
	while (*s)
	{
		if ((*s == '"' || *s == '\'') && !inside_quotes)
			export_stuff(&string_count, &inside_quotes);
		else if ((*s == '"' || *s == '\'') && inside_quotes)
			export_stuff(&string_count, &inside_quotes);
		else if (!strchr(c, *s) && !inside_quotes)
		{
			string_count++;
			while (*s && !strchr(c, *s) && !strchr("\"'", *s))
				s++;
		}
		s++;
	}
	return (string_count);
}

void	*ft_free(char **split_arr, size_t i)
{
	while (i--)
	{
		free(split_arr[i]);
	}
	free(split_arr);
	return (NULL);
}
