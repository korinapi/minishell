/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 14:12:33 by cpuiu             #+#    #+#             */
/*   Updated: 2024/04/06 16:29:47 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

void	process_quote_character(char **input, char quote_char, char **word,
		int *word_len, int *in_quote)
{
	char	nested_quote;
	char	*nested_word;

	if (**input == quote_char)
	{
		*in_quote = !*in_quote;
		(*input)++;
	}
	else if (*in_quote && (**input == '\'' || **input == '"'))
	{
		nested_quote = **input;
		nested_word = parse_quotes(input, nested_quote);
		*word = ft_append_str(*word, word_len, nested_word);
		free(nested_word);
	}
	else
	{
		*word = ft_append_char(*word, word_len, **input);
		(*input)++;
	}
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
	while (**input)
		process_quote_character(input, quote_char, &word, &word_len, &in_quote);
	word = ft_append_char(word, &word_len, '\0');
	return (word);
}
