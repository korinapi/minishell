/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:31:38 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/21 19:51:53 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

char	*parse_redirection_file(char **input)
{
	while (*input && ft_isspace(**input))
		(*input)++;
	if (**input == '\'')
		return (parse_quotes_two(input, '\''));
	else if (**input == '"')
		return (parse_quotes_two(input, '"'));
	else if (**input == '$')
		return (ft_get_variable(input));
	else
		return (ft_get_word(input));
}
void	handle_output_redirection(char **input, t_ast **ast)
{
	int		mode;
	char	*file;
	t_ast	*node;

	file = NULL;
	if ((*input)[1] == '>')
	{
		mode = REDIR_OUT_APPEND;
		(*input)++;
	}
	else
		mode = REDIR_OUT;
	(*input)++;
	while (!file)
		file = parse_redirection_file(input);
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
	
	file = NULL;
	if ((*input)[1] == '<')
	{
		(*input)++;
		mode = REDIR_HEREDOC;
	}
	else
		mode = REDIR_IN;
	(*input)++;
	while (!file)
		file = parse_redirection_file(input);
	node = create_ast_node(AST_REDIRECTION, NULL);
	node->redirection_file = file;
	node->redirection_mode = mode;
	ast_append(*ast, node);
}
// void	handle_redirections(char **input, t_ast *node)
// {
// 	int		mode = 0;
// 	char	*file;

// 	if (**input == '>')
// 	{
// 		mode = (*(*input + 1) == '>') ? REDIR_OUT_APPEND : REDIR_OUT;
// 		(*input) += (*(*input + 1) == '>') ? 2 : 1;
// 	}
// 	else if (**input == '<')
// 	{
// 		mode = (*(*input + 1) == '<') ? REDIR_HEREDOC : REDIR_IN;
// 		(*input) += (*(*input + 1) == '<') ? 2 : 1;
// 	}
// 	file = parse_redirection_file(input);
// 	node = create_ast_node(AST_REDIRECTION, NULL);
// 	node->redirections = realloc(node->redirections, sizeof(t_redirection)
// 			* (node->num_redirections + 1));
// 	node->redirections[node->num_redirections].mode = mode;
// 	node->redirections[node->num_redirections].filename = file;
// 	node->num_redirections++;
// }


void	perform_file_operations(char *file)
{
	char	buf[BUFSIZ];
	ssize_t	bytes_read;
	int		fd_in;
	int		fd_out;

	fd_in = open(file, O_RDONLY);
	if (fd_in != -1)
	{
		fd_out = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd_out != -1)
		{
			bytes_read = read(fd_in, buf, BUFSIZ);
			while (bytes_read != 0)
			{
				bytes_read = read(fd_in, buf, BUFSIZ);
				write(fd_out, buf, bytes_read);
			}
			close(fd_out);
		}
		close(fd_in);
	}
}


void	parse_redirection(char **input, t_ast **ast)
{
	if (**input == '>')
		handle_output_redirection(input, ast);
	else if (**input == '<')
		handle_input_redirection(input, ast);
}
