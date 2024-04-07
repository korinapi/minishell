/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:31:38 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/07 06:03:47 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

void	parse_redirection(char *input, t_ast **ast)
{
	char	*file;
	t_ast	*node;
	int		mode;

	if (*input == '>')
	{
		if (input[1] == '>')
		{
			mode = REDIR_OUT_APPEND;
			input++;
		}
		else
			mode = REDIR_OUT;
		input++;
		while (input && ft_isspace(*input))
			input++;
		file = ft_get_word(&input);
		node = create_ast_node(AST_REDIRECTION, NULL);
		node->redirection_file = file;
		node->redirection_mode = mode;
		ast_append(*ast, node);
	}
	else if (*input == '<')
	{
		if (input[1] == '<')
		{
			mode = REDIR_HEREDOC;
			input++;
		}
		else
			mode = REDIR_IN;
		input++;
		while (input && ft_isspace(*input))
			input++;
		file = ft_get_word(&input);
		node = create_ast_node(AST_REDIRECTION, NULL);
		node->redirection_file = file;
		node->redirection_mode = mode;
		ast_append(*ast, node);
	}
}
