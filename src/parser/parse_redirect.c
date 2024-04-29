/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:31:38 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/29 18:17:08 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

char	*parse_redir_file(char **input)
{
	while (*input && ft_isspace(**input))
		(*input)++;
	if (**input == '\'')
		return (parse_quotes_two(input, '\''));
	else if (**input == '"')
		return (parse_quotes_two(input, '"'));
	else if (**input == '$')
		return (ft_get_variable(input));
	else
		return (ft_get_word(input));
}

void	handle_output_redir(char **input, t_ast **ast)
{
	int		mode;
	char	*file;
	t_ast	*node;

	file = NULL;
	if ((*input)[1] == '>')
	{
		mode = REDIR_OUT_APPEND;
		(*input)++;
	}
	else
		mode = REDIR_OUT;
	(*input)++;
	while (!file)
		file = parse_redir_file(input);
	node = create_ast_node(AST_REDIRECTION, NULL);
	node->redirection_file = file;
	node->redirection_mode = mode;
	ast_append(*ast, node);
}

void	handle_input_redir(char **input, t_ast **ast)
{
	int		mode;
	char	*file;
	t_ast	*node;

	file = NULL;
	if ((*input)[1] == '<')
	{
		(*input)++;
		mode = REDIR_HEREDOC;
	}
	else
		mode = REDIR_IN;
	(*input)++;
	while (!file)
		file = parse_redir_file(input);
	node = create_ast_node(AST_REDIRECTION, NULL);
	node->redirection_file = file;
	node->redirection_mode = mode;
	ast_append(*ast, node);
}

void	parse_redirection(char **input, t_ast **ast)
{
	if (**input == '>')
		handle_output_redir(input, ast);
	else if (**input == '<')
		handle_input_redir(input, ast);
}
