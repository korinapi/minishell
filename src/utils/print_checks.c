/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 23:19:33 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/25 16:44:05 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

void	print_ast(t_ast *node, int level, char *node_type)
{
	if (node == NULL)
	{
		return ;
	}

	for (int i = 0; i < level; i++)
	{
		printf("  ");
	}

	switch (node->type)
	{
	case AST_SIMPLE_COMMAND:
		printf("%s: AST_SIMPLE_COMMAND\n", node_type);
		break ;
	case AST_WORD:
		printf("%s: AST_WORD: %s\n", node_type, node->data);
		break ;
	case AST_SINGLEQUOTED_WORD:
		printf("%s: AST_SINGLEQUOTED_WORD: %s\n", node_type, node->data);
		break ;
	case AST_DOUBLEQUOTED_WORD:
		printf("%s: AST_DOUBLEQUOTED_WORD: %s\n", node_type, node->data);
		break ;
	case AST_VARIABLE:
		printf("%s: AST_VARIABLE: %s\n", node_type, node->data);
		break ;
	case AST_SPECIAL_PARAM:
		printf("%s: AST_SPECIAL_PARAM: %s\n", node_type, node->data);
		break ;
	case AST_WHITESPACE:
		printf("%s: AST_WHITESPACE: file: %s mode: %d, data: %s\n", node_type,
			node->redirection_file, node->redirection_mode, node->data);
		break ;
	case AST_REDIRECTION:
		printf("%s: AST_REDIRECTION: file: %s mode: %d, data: %s\n", node_type,
			node->redirection_file, node->redirection_mode, node->data);
		break ;
	case AST_PIPELINE:
		printf("%s: AST_PIPELINE\n", node_type);
		break ;
	default:
		printf("%s: Unknown node type\n", node_type);
		break ;
	}
	print_ast(node->left, level + 1, "Left");
	print_ast(node->right, level + 1, "Right");
}
