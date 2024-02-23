/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 14:34:02 by cpuiu             #+#    #+#             */
/*   Updated: 2024/01/31 14:34:42 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	handle_quoted_substring(char *str, int *i, int *word_index, char **array)
{
	char	quote;
	int		start_index;

	quote = str[(*i)++];
	start_index = *i;
	while (str[*i] != '\0' && str[*i] != quote)
	{
		++(*i);
	}
	while (start_index < *i && (str[start_index] == ' '
			|| str[start_index] == '\t'))
	{
		++start_index;
	}
	while (*i > start_index && (str[*i - 1] == ' ' || str[*i - 1] == '\t'))
	{
		--(*i);
	}
	array[(*word_index)++] = word_dupe(&str[start_index], *i - start_index);
	if (str[*i] != '\0')
	{
		++(*i);
	}
	return (*i);
}

char	**ft_split_quotes(char *str)
{
	int		i;
	int		word_index;
	char	**array;
	int		start_index;

	i = 0;
	word_index = 0;
	array = malloc(sizeof(char *) * (ft_wordcount(str) + 1));
	while (str[i] != '\0')
	{
		while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
			++i;
		if (is_quote(str[i]))
			i = handle_quoted_substring(str, &i, &word_index, array);
		else
		{
			start_index = i;
			while (str[i] != '\0' && !is_quote(str[i]) && str[i] != ' '
				&& str[i] != '\t' && str[i] != '\n')
				++i;
			array[word_index++] = word_dupe(&str[start_index], i - start_index);
		}
	}
	array[word_index] = 0;
	return (array);
}
