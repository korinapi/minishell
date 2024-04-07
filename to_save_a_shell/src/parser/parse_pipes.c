/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:31:22 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/07 06:03:35 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

void	parse_pipeline(char **input, t_ast **ast)
{
	t_ast	*node;
	char	*start;
	t_ast	*command_node;

	start = *input;
	node = create_ast_node(AST_PIPELINE, NULL);
	*ast = node;
	command_node = NULL;
	while (**input)
	{
		parse_simple_command(input, &node->left);
		if (node->left->type == AST_SIMPLE_COMMAND)
		{
			command_node = node->left;
			while (command_node->right
				&& command_node->right->type == AST_WHITESPACE)
				command_node->right = command_node->right->right;
			while (command_node->left
				&& command_node->left->type == AST_WHITESPACE)
				command_node->left = command_node->left->right;
		}
		if (**input == '|')
		{
			(*input)++;
			node->right = create_ast_node(AST_PIPELINE, NULL);
			node = node->right;
		}
		else if (**input == '>' || **input == '<')
		{
			parse_redirection(*input, &node->left);
			break ;
		}
		else
			break ;
	}
	if (start == *input)
		free_ast(node);
}
