/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:59:44 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/23 21:47:54 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "environment.h"
#include "executor.h"
#include "input.h"
#include "minishell.h"
#include "parser.h"
#include "signals.h"
#include "utilities.h"

// static t_environ_pointers	g_environ_pointers = {0, NULL};

int	startup_check(int argc, char **argv)
{
	(void)(argv);
	if (argc != 1)
	{
		perror("Too many arguments for startup");
		return (1);
	}
	return (0);
}

void	close_main(void)
{
	ft_free_env(environ);
	rl_clear_history();
	printf("exit\n");
}

void	set_man_environ(void)
{
	char	*cwd;
	int		level;
	char	*sh_level;
	char	*sh_level_str;

	sh_level_str = NULL;
	cwd = getcwd(NULL, 0);
	ft_setenv("PWD", cwd, 1);
	sh_level = getenv("SHLVL");
	if (sh_level == NULL || ft_atoi(sh_level) == 0)
		level = 1;
	else
		level = ft_atoi(sh_level) + 1;
	sh_level_str = (char *)malloc(16 * sizeof(char));
	ft_snprintf(sh_level_str, 16, "%d", level);
	ft_setenv("SHLVL", sh_level_str, 1);
	free(cwd);
	free(sh_level_str);
}

int	main(int argc, char **argv)
{
	char	*input;
	t_ast	*ast;
	int		exit_status;

	rl_catch_signals = 0;
	if (startup_check(argc, argv))
		return (1);
	setup_signals();
	exit_status = 0;
	environ = ft_copy_env();
	set_man_environ();
	while (1)
	{
		input = get_input();
		if (!input)
			break ;
		ast = parse_input(input);
		if (ast)
		{
			execute_ast(ast, &exit_status);
			free_ast(ast);
		}
	}
	return (close_main(), exit_status);
}

// print_ast(ast, 0, "Root");