/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:30:18 by cpuiu             #+#    #+#             */
/*   Updated: 2024/04/06 15:43:42 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

void	handle_output_redirection(char **input, t_ast **ast)
{
	int		mode;
	char	*file;
	t_ast	*node;

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

void	handle_input_redirection(char **input, t_ast **ast)
{
	int		mode;
	char	*file;
	t_ast	*node;

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
	file = ft_get_word(input);
	node = create_ast_node(AST_REDIRECTION, NULL);
	node->redirection_file = file;
	node->redirection_mode = mode;
	ast_append(*ast, node);
}

void	parse_redirection(char *input, t_ast **ast)
{
	if (*input == '>')
		handle_output_redirection(&input, ast);
	else if (*input == '<')
		handle_input_redirection(&input, ast);
}
