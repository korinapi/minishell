/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:31:42 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/23 20:09:50 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

void	handle_single_quote_segment(char **input, t_ast **parent, t_ast **prev)
{
	char	*word;
	int		data_len;
	t_ast	*node;

	data_len = 0;
	word = parse_quotes(input, '\'');
	if (!word)
		return ;
	if (*prev && ((*prev)->type == AST_DOUBLEQUOTED_WORD
			|| (*prev)->type == AST_WORD
			|| (*prev)->type == AST_SINGLEQUOTED_WORD) && !(*prev)->right)
	{
		data_len = ft_strlen((*prev)->data);
		(*prev)->data = ft_append_str((*prev)->data, &data_len, word);
		(*prev)->type = AST_SINGLEQUOTED_WORD;
		free(word);
	}
	else
	{
		node = create_ast_node(AST_SINGLEQUOTED_WORD, word);
		ast_append(*parent, node);
		*prev = node;
	}
}

void	handle_double_quote_segment(char **input, t_ast **parent, t_ast **prev)
{
	char	*word;
	int		data_len;
	t_ast	*node;

	word = parse_quotes(input, '"');
	if (!word)
		return ;
	word = handle_variable_expansion(word);
	if (*prev && ((*prev)->type == AST_DOUBLEQUOTED_WORD
			|| (*prev)->type == AST_WORD
			|| (*prev)->type == AST_SINGLEQUOTED_WORD) && !(*prev)->right)
	{
		data_len = ft_strlen((*prev)->data);
		(*prev)->data = ft_append_str((*prev)->data, &data_len, word);
		(*prev)->type = AST_DOUBLEQUOTED_WORD;
		free(word);
	}
	else
	{
		node = create_ast_node(AST_DOUBLEQUOTED_WORD, word);
		ast_append(*parent, node);
		*prev = node;
	}
}

void	handle_unquoted_word(char **input, t_ast **parent, t_ast **prev)
{
	char	*word;
	t_ast	*node;
	int		data_len;

	data_len = 0;
	word = ft_get_word(input);
	if (*prev && ((*prev)->type == AST_WORD
			|| (*prev)->type == AST_DOUBLEQUOTED_WORD
			|| (*prev)->type == AST_SINGLEQUOTED_WORD) && !(*prev)->right)
	{
		data_len = ft_strlen((*prev)->data);
		(*prev)->data = ft_append_str((*prev)->data, &data_len, word);
		free(word);
	}
	else
	{
		node = create_ast_node(AST_WORD, word);
		ast_append(*parent, node);
		*prev = node;
	}
}

void	parse_command_segment(char **input, t_ast **parent, t_ast **prev)
{
	char	*word;
	t_ast	*node;

	while (**input && ft_isspace(**input))
	{
		word = ft_calloc(2, sizeof(char));
		word[0] = **input;
		word[1] = '\0';
		node = create_ast_node(AST_WHITESPACE, word);
		ast_append(*parent, node);
		(*input)++;
	}
	if (!**input || **input == '|')
		return ;
	else if (**input == '>' || **input == '<')
		parse_redirection(input, parent);
	else if (**input == '\'')
		handle_single_quote_segment(input, parent, prev);
	else if (**input == '"')
		handle_double_quote_segment(input, parent, prev);
	else if (**input == '$')
		handle_variable_parsing(input, parent, prev);
	else
		handle_unquoted_word(input, parent, prev);
}
