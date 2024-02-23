/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:59:37 by mleibeng          #+#    #+#             */
/*   Updated: 2024/02/22 12:19:55 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <curses.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>

# define MAX_ARGS 262144
# define DELIMITERS " \n\r\t\a"
# define INIT_HASH	100
# define HASH_MULT	2
# define MAX_COMMAND_LENGTH 4096
# define IDENTIFIER "is not a valid identifier"

extern volatile sig_atomic_t interrupt_process;
// extern char **environ; // necessary for export later

typedef struct	e_key
{
	char	*key;
	char	*value;
	char	*string;
	struct	e_key *next;
	
}				t_key;

typedef struct	e_hash
{
	t_key	*table;
	int		hash_size;
	int		hash_cap;
}				t_hash;

typedef struct s_hashdata
{
	size_t	key_len;
	char	*start;
	char	*end;
	char	*key;
	char	*var;
	char	*val;
}				t_hashdata;

typedef enum e_type
{
	COMMAND,
	ERROR,
	OPT,
	ARG,
	VAR,
	SPEC,
	EDGE,
} t_type;

typedef struct s_token
{
	t_type	type;
	char	*path_command;
	int		pos;
	char	*value;
	bool	s_quoted;
	bool	d_quoted;
} t_token;

typedef struct s_data
{
	t_token 	*tokens;
	size_t		num_of_elements;
	char		*input;
	int			exitcode;
	char 		**envp;
	t_hash		*var_hash;
	t_hashdata	*hash_data;

} t_data;

int		exit_func(t_data *data);
void	init_signals(void);
int		read_in(const char *input_line, t_data *data);
void	get_pwd(void);
void	parse_tokens(char **split_array, t_data *data);
void 	create_hash(t_data *data, size_t init_size);
int		assign_hash_pos(char *key, size_t hash_cap);
void	create_key(t_data *data, char *occ);


#endif