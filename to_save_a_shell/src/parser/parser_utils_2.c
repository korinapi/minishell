/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 14:12:29 by cpuiu             #+#    #+#             */
/*   Updated: 2024/04/06 16:29:39 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

char	*expand_variable(char *word, char **new_word, int *word_len)
{
	char	*var_end;
	char	*var_name;
	char	*var_value;

	var_end = word + 1;
	while (*var_end && ft_isalnum(*var_end))
		var_end++;
	if (var_end == word + 1)
		*new_word = ft_append_char(*new_word, word_len, *word);
	else
	{
		var_name = ft_substr(word + 1, 0, var_end - word - 1);
		var_value = getenv(var_name);
		free(var_name);
		if (var_value)
			*new_word = ft_append_str(*new_word, word_len, var_value);
	}
	return (var_end);
}

char	*handle_variable_expansion(char *word)
{
	char	*new_word;
	int		word_len;

	new_word = NULL;
	word_len = 0;
	while (*word)
	{
		if (*word == '$' && *(word + 1) != '?' && !ft_isspace(*(word + 1)))
			word = expand_variable(word, &new_word, &word_len);
		else
		{
			new_word = ft_append_char(new_word, &word_len, *word);
			word++;
		}
	}
	new_word = ft_append_char(new_word, &word_len, '\0');
	return (new_word);
}

void	ast_append(t_ast *parent, t_ast *child)
{
	t_ast	*current;

	if (!parent || !child)
		return ;
	if (!parent->left)
	{
		parent->left = child;
		return ;
	}
	current = parent->left;
	while (current->right)
		current = current->right;
	current->right = child;
}

t_ast	*create_ast_node(t_ast_type type, char *data)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	node->type = type;
	node->data = data;
	node->redirection_file = NULL;
	node->redirection_mode = 0;
	node->left = NULL;
	node->right = NULL;
	return (node);
}
