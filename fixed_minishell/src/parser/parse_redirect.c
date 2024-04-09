/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:31:38 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/08 12:18:00 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

void handle_output_redirection(char **input, t_ast **ast)
{
	int mode;
	char *file;
	t_ast *node;

	if ((*input)[1] == '>')
	{
		mode = REDIR_OUT_APPEND;
		(*input)++;
	}
	else
		mode = REDIR_OUT;
	(*input)++;
	while (*input && ft_isspace(**input))
		(*input)++;
	file = ft_get_word(input);
	node = create_ast_node(AST_REDIRECTION, NULL);
	node->redirection_file = file;
	node->redirection_mode = mode;
	ast_append(*ast, node);
}

void handle_input_redirection(char **input, t_ast **ast)
{
	int mode;
	char *file;
	t_ast *node;

	if ((*input)[1] == '<')
	{
		mode = REDIR_HEREDOC;
		(*input)++;
	}
	else
		mode = REDIR_IN;
	(*input)++;
	while (*input && ft_isspace(**input))
		(*input)++;

	switch (**input)
	{
	case '\'':
		file = parse_quotes(input, '\'');
		break;
	case '"':
		file = parse_quotes(input, '"');
		break;
	case '$':
		file = ft_get_variable(input);
		break;
	default:
		file = ft_get_word(input);
		break;
	}

	node = create_ast_node(AST_REDIRECTION, NULL);
	node->redirection_file = file;
	node->redirection_mode = mode;
	ast_append(*ast, node);
}

void parse_redirection(char *input, t_ast **ast)
{
	if (*input == '>')
		handle_output_redirection(&input, ast);
	else if (*input == '<')
		handle_input_redirection(&input, ast);
}
