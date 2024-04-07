/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:32:29 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/07 06:03:41 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

char	*parse_quotes(char **input, char quote_char)
{
	char	*word;
	int		word_len;
	char	nested_quote;
	char	*nested_word;
	int		in_quote;

	word = NULL;
	word_len = 0;
	in_quote = 0;
	(*input)++;
	while (**input)
	{
		if (**input == quote_char)
		{
			in_quote = !in_quote;
			(*input)++;
		}
		else if (in_quote)
		{
			if (**input == '\'' || **input == '"')
			{
				nested_quote = **input;
				nested_word = parse_quotes(input, nested_quote);
				word = ft_append_str(word, &word_len, nested_word);
				free(nested_word);
			}
			else
			{
				word = ft_append_char(word, &word_len, **input);
				(*input)++;
			}
		}
		else
		{
			word = ft_append_char(word, &word_len, **input);
			(*input)++;
		}
	}
	word = ft_append_char(word, &word_len, '\0');
	return (word);
}
