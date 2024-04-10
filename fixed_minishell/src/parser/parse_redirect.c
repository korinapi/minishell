/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:31:38 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/10 01:00:49 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

void	handle_output_redirection(char **input, t_ast **ast)
{
	int		mode;
	char	*file;
	t_ast	*node;

	if ((*input)[1] == '>')
	{
		mode = REDIR_OUT_APPEND;
		(*input)++;
	}
	else
		mode = REDIR_OUT;
	(*input)++;
	while (*input && ft_isspace(**input))
		(*input)++;
	file = ft_get_word(input);
	node = create_ast_node(AST_REDIRECTION, NULL);
	node->redirection_file = file;
	node->redirection_mode = mode;
	ast_append(*ast, node);
}

void	handle_input_redirection(char **input, t_ast **ast)
{
	int		mode;
	char	*file;
	t_ast	*node;
				char buf[BUFSIZ];
				ssize_t bytes_read;

	int fd_in, fd_out;
	if ((*input)[1] == '<')
	{
		mode = REDIR_HEREDOC;
		(*input)++;
	}
	else
	{
		mode = REDIR_IN;
	}
	(*input)++;
	while (*input && ft_isspace(**input))
	{
		(*input)++;
	}
	if (mode == REDIR_IN)
	{
		if (**input == '\'')
		{
			file = parse_quotes_two(input, '\'');
		}
		else if (**input == '"')
		{
			file = parse_quotes_two(input, '"');
		}
		else if (**input == '$')
		{
			file = ft_get_variable(input);
		}
		else
		{
			file = ft_get_word(input);
		}
		fd_in = open(file, O_RDONLY);
		if (fd_in != -1)
		{
			fd_out = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd_out != -1)
			{
				while ((bytes_read = read(fd_in, buf, BUFSIZ)) > 0)
				{
					write(fd_out, buf, bytes_read);
				}
				close(fd_out);
			}
			close(fd_in);
		}
	}
	else
	{
		if (**input == '\'')
		{
			file = parse_quotes_two(input, '\'');
		}
		else if (**input == '"')
		{
			file = parse_quotes_two(input, '"');
		}
		else if (**input == '$')
		{
			file = ft_get_variable(input);
		}
		else
		{
			file = ft_get_word(input);
		}
	}
	node = create_ast_node(AST_REDIRECTION, NULL);
	node->redirection_file = file;
	node->redirection_mode = mode;
	ast_append(*ast, node);
}

void	parse_redirection(char **input, t_ast **ast)
{
	if (**input == '>')
		handle_output_redirection(input, ast);
	else if (**input == '<')
		handle_input_redirection(input, ast);
}
