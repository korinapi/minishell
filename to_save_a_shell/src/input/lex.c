/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:59:49 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/06 12:18:41 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "minishell.h"

char	*get_input(void)
{
	char	*input;

	input = readline("minishell> ");
	if (input && *input)
		add_history(input);
	return (input);
}
