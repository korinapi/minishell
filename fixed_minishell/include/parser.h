/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:26:02 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/10 00:24:37 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# define REDIR_OUT 1
# define REDIR_OUT_APPEND 2
# define REDIR_IN 3
# define REDIR_HEREDOC 4

typedef enum e_ast_type
{
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

t_ast				*create_ast_node(t_ast_type type, char *data);
int					ast_count_nodes(t_ast *ast);
t_ast				*parse_input(char *input);
void				free_ast(t_ast *ast);
void				print_ast(t_ast *node, int level, char *node_type);
char				*ft_append_char(char *str, int *len, char c);
char				*ft_append_str(char *str1, int *len1, const char *str2);
void				ast_append(t_ast *parent, t_ast *child);
/*get vars and expand*/
char				*ft_get_word(char **input);
char				*handle_variable_expansion(char *word);
char				*ft_get_variable(char **input);
/*Parse functions*/
void				parse_simple_command(char **input, t_ast **ast);
char				*parse_quotes(char **input, char quote_char);
char				*parse_quotes_two(char **input, char quote_char);
void				parse_redirection(char **input, t_ast **ast);
void				handle_output_redirection(char **input, t_ast **ast);
void				handle_input_redirection(char **input, t_ast **ast);
void				parse_pipeline(char **input, t_ast **ast);
void				parse_command_segment(char **input, t_ast **parent,
						t_ast **prev);

#endif