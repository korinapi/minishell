/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:32:29 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/12 08:10:39 by mleibeng         ###   ########.fr       */
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
		{
			if (*word)
				*word = ft_append_str(*word, word_len, parse_n(input, **input));
			else
				*word = parse_n(input, **input);
		}
		else
		{
			if (*word)
				*word = ft_append_char(*word, word_len, **input);
			else
			{
				*word = malloc(sizeof(char));
				**word = **input;
				*word_len = 1;
			}
			(*input)++;
		}
	}
	else
	{
		if (*word)
			*word = ft_append_char(*word, word_len, **input);
		else
		{
			*word = malloc(sizeof(char));
			**word = **input;
			*word_len = 1;
		}
		(*input)++;
	}
}

char	*parse_quotes_two(char **input, char quote_char)
{
	char	*word;
	int		word_len;
	char	*startingQuote;
	char	*endingQuote;

	word = NULL;
	word_len = 0;
	startingQuote = NULL;
	endingQuote = NULL;
	while (**input && !endingQuote)
	{
		if (**input == quote_char)
		{
			if (NULL == startingQuote)
				startingQuote = (*input + 1);
			else if (*(*input + 1) == quote_char)
				(*input)++;
			else
				endingQuote = *input;
		}
		else
			word = ft_append_char(word, &word_len, **input);
		(*input)++;
	}
	return (word);
}

char	*parse_quotes(char **input, char quote_char)
{
	char	*word;
	int		word_len;
	int		in_quote;

	word = NULL;
	word_len = 0;
	in_quote = 0;
	(*input)++;
	while (**input && in_quote != 1)
	{
		if (**input == quote_char)
		{
			if (*(*input + 1) == quote_char)
				(*input) += 2;
			else
				(*input)++;
			in_quote = !in_quote;
		}
		else
			handle_nested(input, &word, &word_len, &in_quote);
	}
	if (word)
		word = ft_append_char(word, &word_len, '\0');
	return (word);
}
