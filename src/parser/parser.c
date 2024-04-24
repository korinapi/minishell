/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:29:03 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/24 00:45:24 by mleibeng         ###   ########.fr       */
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

t_ast	*parse_input(char *input, char **envp)
{
	t_ast	*ast;

	if (is_input_empty(input))
	{
		free(input);
		input = NULL;
		return (NULL);
	}
	ast = NULL;
	if (has_pipes(input))
		parse_pipeline(&input, &ast, envp);
	else
		parse_simple_command(&input, &ast, envp);
	return (ast);
}

void	parse_simple_command(char **input, t_ast **ast, char **envp)
{
	t_ast	*parent;
	t_ast	*prev;

	parent = create_ast_node(AST_SIMPLE_COMMAND, NULL);
	*ast = parent;
	prev = NULL;
	while (**input)
	{
		parse_command_segment(input, &parent, &prev, envp);
		if (!**input || **input == '|')
			break ;
	}
}
