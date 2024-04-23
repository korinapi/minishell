/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:31:42 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/23 19:57:34 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

// void	parse_simple_command(char **input, t_ast **ast)
// {
// 	char	*word;
// 	t_ast	*node;
// 	t_ast	*parent;
// 	t_ast	*prev;
// 	int		data_len;

// 	parent = create_ast_node(AST_SIMPLE_COMMAND, NULL);
// 	*ast = parent;
// 	prev = NULL;
// 	data_len = 0;
// 	while (**input)
// 	{
// 		while (**input && ft_isspace(**input))
// 		{
// 			word = ft_calloc(2, sizeof(char));
// 			word[0] = **input;
// 			word[1] = '\0';
// 			node = create_ast_node(AST_WHITESPACE, word);
// 			ast_append(parent, node);
// 			(*input)++;
// 		}
// 		if (!**input || **input == '|' || **input == '>' || **input == '<')
// 			break ;
// 		if (**input == '\'')
// 		{
// 			word = parse_quotes(input, '\'');
// 			if (prev && (prev->type == AST_SINGLEQUOTED_WORD
// 					|| prev->type == AST_WORD) && !prev->right)
// 			{
// 				data_len = ft_strlen(prev->data);
// 				prev->data = ft_append_str(prev->data, &data_len, word);
// 				prev->type = AST_SINGLEQUOTED_WORD;
// 				free(word);
// 			}
// 			else
// 			{
// 				node = create_ast_node(AST_SINGLEQUOTED_WORD, word);
// 				ast_append(parent, node);
// 				prev = node;
// 			}
// 		}
// 		else if (**input == '"')
// 		{
// 			word = parse_quotes(input, '"');
// 			word = handle_variable_expansion(word);
// 			if (prev && (prev->type == AST_DOUBLEQUOTED_WORD
// 					|| prev->type == AST_WORD) && !prev->right)
// 			{
// 				data_len = ft_strlen(prev->data);
// 				prev->data = ft_append_str(prev->data, &data_len, word);
// 				prev->type = AST_DOUBLEQUOTED_WORD;
// 				free(word);
// 			}
// 			else
// 			{
// 				node = create_ast_node(AST_DOUBLEQUOTED_WORD, word);
// 				ast_append(parent, node);
// 				prev = node;
// 			}
// 		}
// 		else if (**input == '$')
// 		{
// 			if (*(*input + 1) && *(*input + 1) == '?')
// 			{
// 				word = ft_strdup("$?");
// 				node = create_ast_node(AST_SPECIAL_PARAM, word);
// 				ast_append(parent, node);
// 				(*input) += 2;
// 				continue ;
// 			}
// 			else if ((*(*input + 1) && ft_isspace(*(*input + 1))) || !*(*input
// 					+ 1))
// 			{
// 				word = ft_strdup("$");
// 				node = create_ast_node(AST_WORD, word);
// 				ast_append(parent, node);
// 				(*input)++;
// 				continue ;
// 			}
// 			word = ft_get_variable(input);
// 			node = create_ast_node(AST_VARIABLE, word);
// 			ast_append(parent, node);
// 			prev = node;
// 		}
// 		else
// 		{
// 			word = ft_get_word(input);
// 			if (prev && (prev->type == AST_WORD
// 					|| prev->type == AST_DOUBLEQUOTED_WORD
// 					|| prev->type == AST_SINGLEQUOTED_WORD) && !prev->right)
// 			{
// 				data_len = ft_strlen(prev->data);
// 				prev->data = ft_append_str(prev->data, &data_len, word);
// 				free(word);
// 			}
// 			else
// 			{
// 				node = create_ast_node(AST_WORD, word);
// 				ast_append(parent, node);
// 				prev = node;
// 			}
// 		}
// 	}
// }

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

void	handle_variable_parsing(char **input, t_ast **parent, t_ast **prev)
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
		word = ft_get_variable(input);
		word = handle_variable_expansion(word);
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
