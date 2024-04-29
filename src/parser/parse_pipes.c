/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:31:22 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/29 18:14:29 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

int	handle_pipe_cont_or_redir(char **input, t_ast **node)
{
	if (**input == '|')
	{
		(*input)++;
		(*node)->right = create_ast_node(AST_PIPELINE, NULL);
		*node = (*node)->right;
	}
	else if (**input == '>' || **input == '<')
	{
		parse_redirection(input, &(*node)->left);
		return (1);
	}
	else
		return (1);
	return (0);
}

void	parse_pipeline(char **input, t_ast **ast, char **envp)
{
	t_ast	*node;
	char	*start;

	start = *input;
	node = create_ast_node(AST_PIPELINE, NULL);
	*ast = node;
	while (**input)
	{
		parse_simple_command(input, &node->left, envp);
		if (handle_pipe_cont_or_redir(input, &node))
			break ;
	}
	if (start == *input)
		free_ast(node);
}
