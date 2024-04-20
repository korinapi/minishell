/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:59:49 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/07 04:30:21 by mleibeng         ###   ########.fr       */
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
