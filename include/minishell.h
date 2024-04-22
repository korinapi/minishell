/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:59:37 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/20 23:22:39 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "pipex.h"
# include <curses.h>
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>

# define MAX_ARGS 262144
# define DELIMITERS " \n\r\t\a"
# define MAX_PATH 4096
# define SYNTAX "minishell: syntax error near unexpected token"
# define QUOTES "minishell: unclosed quotes"
# define MINISHELL "minishell"
# define IDENTIFIER "is not a valid identifier"
# define S_QUOTE '\''
# define D_QUOTE '"'
# define NO_PERM "Permission denied"
# define NOT_FOUND "Not found"
# define DIR "is a directory"
# define NO_DIR "no directory"

typedef struct s_handmedown
{
	char	**alloc_env;
	int		num_of_env;
	int		*exitcode;
}	t_handmedown;

extern volatile sig_atomic_t	g_sigint_received;
extern char						**environ;

#endif