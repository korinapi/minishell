/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 20:08:51 by cpuiu             #+#    #+#             */
/*   Updated: 2024/04/24 00:44:03 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

char	*get_and_expand_variable(char **input, char **envp)
{
	char	*word;

	word = ft_get_variable(input);
	return (handle_variable_expansion(word, envp));
}

void	merge_or_create_node(char *word, t_ast **prev, t_ast **parent,
		int *data_len)
{
	t_ast	*node;

	if (*prev && ((*prev)->type == AST_WORD
			|| (*prev)->type == AST_DOUBLEQUOTED_WORD
			|| (*prev)->type == AST_SINGLEQUOTED_WORD) && !(*prev)->right)
	{
		*data_len = ft_strlen((*prev)->data);
		(*prev)->data = ft_append_str((*prev)->data, data_len, word);
		free(word);
	}
	else
	{
		node = create_ast_node(AST_WORD, word);
		ast_append(*parent, node);
		*prev = node;
	}
}

void	handle_variable_parsing(char **input, t_ast **parent, t_ast **prev, char **envp)
{
	char	*word;
	t_ast	*node;
	int		data_len;

	if (*(*input + 1) == '?')
	{
		word = ft_strdup("$?");
		node = create_ast_node(AST_SPECIAL_PARAM, word);
		ast_append(*parent, node);
		(*input) += 2;
	}
	else if ((*(*input + 1) && ft_isspace(*(*input + 1))) || !*(*input + 1))
	{
		word = ft_strdup("$");
		node = create_ast_node(AST_WORD, word);
		ast_append(*parent, node);
		(*input)++;
	}
	else
	{
		word = get_and_expand_variable(input, envp);
		merge_or_create_node(word, prev, parent, &data_len);
	}
}
