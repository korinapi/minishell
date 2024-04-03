/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:26:02 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/02 04:55:46 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

# define REDIR_OUT 1
# define REDIR_OUT_APPEND 2
# define REDIR_IN 3
# define REDIR_HEREDOC 4

typedef enum e_ast_type
{
	AST_COMMAND,
	AST_PIPELINE,
	AST_REDIRECTION,
	AST_SIMPLE_COMMAND,
	AST_WORD,
	AST_SINGLEQUOTED_WORD,
	AST_DOUBLEQUOTED_WORD,
	AST_VARIABLE,
	AST_SPECIAL_PARAM,
	AST_WHITESPACE,
}					t_ast_type;

typedef struct s_ast
{
	t_ast_type		type;
	char			*data;
	char			*redirection_file;
	int				redirection_mode;
	char			*heredoc_delimiter;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

int					ast_count_nodes(t_ast *ast);
t_ast				*parse_input(char *input);
void				free_ast(t_ast *ast);
void				print_ast(t_ast *node, int level, char *node_type);

#endif