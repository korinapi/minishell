/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:29:03 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/07 08:16:41 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

static int	is_input_empty(char *input)
{
	while (*input)
	{
		if (!ft_isspace(*input))
			return (0);
		input++;
	}
	return (1);
}

static int	has_pipes(char *input)
{
	int	in_quote;
	int	in_quote;

	in_quote = 0;
	while (*input)
	{
		if (*input == '"' || *input == '\'')
			in_quote = !in_quote;
		else if (*input == '|' && !in_quote)
			return (1);
		input++;
	}
	return (0);
}

t_ast	*parse_input(char *input)
{
	t_ast	*ast;

	if (is_input_empty(input))
	{
		free(input);
		return (NULL);
	}
	ast = NULL;
	if (has_pipes(input))
		parse_pipeline(&input, &ast);
	else
	{
		parse_simple_command(&input, &ast);
		parse_redirection(input, &ast);
	}
	return (ast);
}
