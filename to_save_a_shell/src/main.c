/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:59:44 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/03 14:26:38 by mleibeng         ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	char	*input;
	t_ast	*ast;
	int		exit_status;

	(void)(argv);
	if (argc != 1)
	{
		perror("Too many arguments for startup");
		return (-1);
	}
	setup_signals();
	exit_status = 0;
	environ = ft_copy_env();
	while (1)
	{
		g_sigint_received = 0;
		input = get_input();
		if (input == NULL)
			exit(exit_status);
		ast = parse_input(input);
		if (ast)
		{
			// print_ast(ast, 0, "Root");
			execute_ast(ast, &exit_status);
			free_ast(ast);
		}
	}
	ft_free_env(environ);
	return (exit_status);
}
