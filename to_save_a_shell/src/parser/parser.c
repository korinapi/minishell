/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:29:03 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/06 16:29:10 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

void	parse_simple_command(char **input, t_ast **ast)
{
	t_ast	*parent;
	t_ast	*prev;
	int		data_len;

	parent = create_ast_node(AST_SIMPLE_COMMAND, NULL);
	*ast = parent;
	prev = NULL;
	data_len = 0;
	while (**input)
	{
		parse_command_segment(input, &parent, &prev);
		if (!**input || **input == '|' || **input == '>' || **input == '<')
			break ;
	}
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
	int	in_quote;

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
