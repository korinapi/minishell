/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:59:44 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/29 17:22:12 by mleibeng         ###   ########.fr       */
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

void	close_main(char **envp)
{
	ft_free_env(envp);
	rl_clear_history();
	printf("exit\n");
}

void	set_non_env(char ***envp, char *cwd)
{
	int		level;
	char	*sh_level;
	char	*sh_level_str;

	ft_setenv("PWD", cwd, 1, envp);
	sh_level = getenv("SHLVL");
	if (sh_level == NULL || ft_atoi(sh_level) == 0)
		level = 1;
	else
		level = ft_atoi(sh_level) + 1;
	sh_level_str = (char *)malloc(16 * sizeof(char));
	ft_snprintf(sh_level_str, 16, "%d", level);
	ft_setenv("SHLVL", sh_level_str, 1, envp);
	free(sh_level_str);
}

void	set_man_environ(char ***envp)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!*envp)
	{
		*envp = ft_calloc(3, sizeof(char *));
		if (!*envp)
		{
			free(cwd);
			return ;
		}
		(*envp)[0] = ft_strjoin("PWD=", cwd);
		(*envp)[1] = ft_strjoin("SHLVL=1", "");
		(*envp)[2] = NULL;
	}
	else
		set_non_env(envp, cwd);
	free(cwd);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_ast	*ast;
	char	**my_envp;
	int		exit_status;

	rl_catch_signals = 0;
	if (startup_check(argc, argv))
		return (1);
	setup_signals();
	exit_status = 0;
	my_envp = ft_copy_env(envp);
	set_man_environ(&my_envp);
	while (1)
	{
		input = get_input();
		if (!input)
			break ;
		ast = parse_input(input, my_envp);
		if (ast)
		{
			execute_ast(ast, &exit_status, &my_envp);
			free_ast(ast);
		}
	}
	return (close_main(my_envp), exit_status);
}
