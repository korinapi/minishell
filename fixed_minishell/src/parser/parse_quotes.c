/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:32:29 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/07 09:39:36 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

char	*parse_n(char **input, char quote_char)
{
	char	*word;
	int		word_len;
	char	nested_quote;

	word = NULL;
	word_len = 0;
	while (**input)
	{
		if (**input == quote_char)
		{
			(*input)++;
			return (word);
		}
		else if (**input == '\'' || **input == '"')
		{
			nested_quote = **input;
			word = ft_append_char(word, &word_len, **input);
			word = ft_append_str(word, &word_len, parse_n(input, nested_quote));
		}
		else
			word = ft_append_char(word, &word_len, **input);
		(*input)++;
	}
	return (word);
}

void	handle_nested(char **input, char **word, int *word_len, int *in_quote)
{
	if (*in_quote)
	{
		if (**input == '\'' || **input == '"')
			*word = ft_append_str(*word, word_len, parse_n(input, **input));
		else
		{
			*word = ft_append_char(*word, word_len, **input);
			(*input)++;
		}
	}
	else
	{
		*word = ft_append_char(*word, word_len, **input);
		(*input)++;
	}
}

char	*parse_quotes(char **input, char quote_char)
{
	char	*word = NULL;
	int		word_len = 0;
	char    *startingQuote = NULL;
	char	*endingQuote = NULL;


	while (**input && !endingQuote)
	{
		if (**input == quote_char)
		{
			if(NULL == startingQuote)
				startingQuote = (*input + 1);
			else if(*(*input + 1) == quote_char)
				(*input)++; // skip the next quote
			else
				endingQuote = *input;
		}
		else
			word = ft_append_char(word, &word_len, **input);

		(*input)++;
	}

	return (word);
}