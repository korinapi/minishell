/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:31:22 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/07 09:10:37 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

void	trim_whitespace_from_command_node(t_ast **command_node)
{
	while ((*command_node)->right
		&& (*command_node)->right->type == AST_WHITESPACE)
		(*command_node)->right = (*command_node)->right->right;
	while ((*command_node)->left
		&& (*command_node)->left->type == AST_WHITESPACE)
		(*command_node)->left = (*command_node)->left->right;
}

int	handle_pipeline_continuation_or_redirection(char **input, t_ast **node)
{
	if (**input == '|')
	{
		(*input)++;
		(*node)->right = create_ast_node(AST_PIPELINE, NULL);
		*node = (*node)->right;
	}
	else if (**input == '>' || **input == '<')
	{
		parse_redirection(*input, &(*node)->left);
		return (1);
	}
	else
		return (1);
	return (0);
}

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
			trim_whitespace_from_command_node(&command_node);
		}
		if (handle_pipeline_continuation_or_redirection(input, &node))
			break ;
	}
	if (start == *input)
		free_ast(node);
}
