/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:29:03 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/05 00:28:06 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

char	*ft_append_char(char *str, int *len, char c)
{
	char	*new_str;
	int		new_len;

	new_len = *len + 1;
	new_str = malloc(new_len + 1);
	if (!new_str)
		return (NULL);
	if (str)
	{
		ft_memcpy(new_str, str, *len);
		free(str);
	}
	new_str[*len] = c;
	new_str[new_len] = '\0';
	*len = new_len;
	return (new_str);
}

char	*ft_append_str(char *str1, int *len1, const char *str2)
{
	char	*new_str;
	int		len2;
	int		new_len;

	len2 = ft_strlen(str2);
	new_len = *len1 + len2;
	new_str = malloc(new_len + 1);
	if (!new_str)
		return (NULL);
	if (str1)
	{
		ft_memcpy(new_str, str1, *len1);
		free(str1);
	}
	ft_memcpy(new_str + *len1, str2, len2);
	new_str[new_len] = '\0';
	*len1 = new_len;
	return (new_str);
}

int	ast_count_nodes(t_ast *ast)
{
	int	count;

	count = 1;
	if (ast == NULL)
		return (0);
	count += ast_count_nodes(ast->left);
	count += ast_count_nodes(ast->right);
	return (count);
}

char	*ft_get_variable(char **input)
{
	char	*var;
	int		len;
	int		i;

	len = 1;
	while ((*input)[len] && (ft_isalnum((*input)[len]) || (*input)[len] == '_'))
		len++;
	var = ft_calloc(len + 1, sizeof(char));
	var[0] = '$';
	i = 1;
	while (i < len)
	{
		var[i] = (*input)[i];
		i++;
	}
	var[i] = '\0';
	*input += len;
	return (var);
}

static char	*handle_variable_expansion(char *word)
{
	char	*new_word;
	int		word_len;
	char	*var_end;
	char	*var_name;
	char	*var_value;

	new_word = NULL;
	word_len = 0;
	while (*word)
	{
		if (*word == '$' && *(word + 1) != '?' && !ft_isspace(*(word + 1)))
		{
			var_end = word + 1;
			while (*var_end && ft_isalnum(*var_end))
				var_end++;
			if (var_end == word + 1)
			{
				new_word = ft_append_char(new_word, &word_len, *word);
				word++;
				continue ;
			}
			var_name = ft_substr(word + 1, 0, var_end - word - 1);
			var_value = getenv(var_name);
			free(var_name);
			if (var_value)
				new_word = ft_append_str(new_word, &word_len, var_value);
			word = var_end;
		}
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

static char	*parse_quotes(char **input, char quote_char)
{
	char	*word;
	int		word_len;
	char	nested_quote;
	char	*nested_word;
	int		in_quote;

	word = NULL;
	word_len = 0;
	in_quote = 0;
	(*input)++;
	while (**input)
	{
		if (**input == quote_char)
		{
			in_quote = !in_quote;
			(*input)++;
		}
		else if (in_quote)
		{
			if (**input == '\'' || **input == '"')
			{
				nested_quote = **input;
				nested_word = parse_quotes(input, nested_quote);
				word = ft_append_str(word, &word_len, nested_word);
				free(nested_word);
			}
			else
			{
				word = ft_append_char(word, &word_len, **input);
				(*input)++;
			}
		}
		else
		{
			word = ft_append_char(word, &word_len, **input);
			(*input)++;
		}
	}
	word = ft_append_char(word, &word_len, '\0');
	return (word);
}

char	*ft_get_word(char **input)
{
	char	*word;
	int		len;
	int		i;

	len = 0;
	word = NULL;
	while ((*input)[len] && !ft_isspace((*input)[len]) && (*input)[len] != '\''
		&& (*input)[len] != '"' && (*input)[len] != '$' && (*input)[len] != '|'
		&& (*input)[len] != '>' && (*input)[len] != '<')
		len++;
	word = ft_calloc(len + 1, sizeof(char));
	i = 0;
	while (i < len)
	{
		word[i] = (*input)[i];
		i++;
	}
	word[i] = '\0';
	*input += len;
	return (word);
}

static void	parse_simple_command(char **input, t_ast **ast)
{
	char	*word;
	t_ast	*node;
	t_ast	*parent;
	t_ast	*prev;
	int		data_len;

	parent = create_ast_node(AST_SIMPLE_COMMAND, NULL);
	*ast = parent;
	prev = NULL;
	data_len = 0;
	while (**input)
	{
		while (**input && ft_isspace(**input))
		{
			word = ft_calloc(2, sizeof(char));
			word[0] = **input;
			word[1] = '\0';
			node = create_ast_node(AST_WHITESPACE, word);
			ast_append(parent, node);
			(*input)++;
		}
		if (!**input || **input == '|' || **input == '>' || **input == '<')
			break ;
		if (**input == '\'')
		{
			word = parse_quotes(input, '\'');
			if (prev && (prev->type == AST_SINGLEQUOTED_WORD
					|| prev->type == AST_WORD) && !prev->right)
			{
				data_len = ft_strlen(prev->data);
				prev->data = ft_append_str(prev->data,
						&data_len, word);
				prev->type = AST_SINGLEQUOTED_WORD;
				free(word);
			}
			else
			{
				node = create_ast_node(AST_SINGLEQUOTED_WORD, word);
				ast_append(parent, node);
				prev = node;
			}
		}
		else if (**input == '"')
		{
			word = parse_quotes(input, '"');
			word = handle_variable_expansion(word);
			if (prev && (prev->type == AST_DOUBLEQUOTED_WORD
					|| prev->type == AST_WORD) && !prev->right)
			{
				data_len = ft_strlen(prev->data);
				prev->data = ft_append_str(prev->data,
						&data_len, word);
				prev->type = AST_DOUBLEQUOTED_WORD;
				free(word);
			}
			else
			{
				node = create_ast_node(AST_DOUBLEQUOTED_WORD, word);
				ast_append(parent, node);
				prev = node;
			}
		}
		else if (**input == '$')
		{
			if (*(*input + 1) == '?')
			{
				word = ft_strdup("$?");
				node = create_ast_node(AST_SPECIAL_PARAM, word);
				ast_append(parent, node);
				(*input) += 2;
				continue ;
			}
			else if ((*(*input + 1) && ft_isspace(*(*input + 1))) || !*(*input
					+ 1))
			{
				word = ft_strdup("$");
				node = create_ast_node(AST_WORD, word);
				ast_append(parent, node);
				(*input)++;
				continue ;
			}
			word = ft_get_variable(input);
			node = create_ast_node(AST_VARIABLE, word);
			ast_append(parent, node);
			prev = node;
		}
		else
		{
			word = ft_get_word(input);
			if (prev && (prev->type == AST_WORD
					|| prev->type == AST_DOUBLEQUOTED_WORD
					|| prev->type == AST_SINGLEQUOTED_WORD) && !prev->right)
			{
				printf("1\n");
				data_len = ft_strlen(prev->data);
				prev->data = ft_append_str(prev->data, &data_len, word);
				free(word);
			}
			else
			{
				node = create_ast_node(AST_WORD, word);
				ast_append(parent, node);
				prev = node;
			}
		}
	}
}

static void	parse_redirection(char *input, t_ast **ast)
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

static void	parse_pipeline(char **input, t_ast **ast)
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

static int	is_input_empty(char *input)
{
	while (*input)
	{
		if (!ft_isspace(*input))
			return (0);
		input++;
	}
	return (1);
}

static int	has_pipes(char *input)
{
	int in_quote;

	in_quote = 0;
	while (*input)
	{
		if (*input == '"' || *input == '\'')
			in_quote = !in_quote;
		else if (*input == '|' && !in_quote)
			return (1);
		input++;
	}
	return (0);
}

t_ast	*parse_input(char *input)
{
	t_ast	*ast;

	if (is_input_empty(input))
	{
		free(input);
		return (NULL);
	}
	ast = NULL;
	if (has_pipes(input))
		parse_pipeline(&input, &ast);
	else
	{
		parse_simple_command(&input, &ast);
		parse_redirection(input, &ast);
	}
	return (ast);
}
