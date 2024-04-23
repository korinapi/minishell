/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 00:22:57 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/04/23 18:13:29 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "free.h"
#include "minishell.h"

void	ft_free_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while(env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	ft_free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	free_ast(ast->left);
	free_ast(ast->right);
	if (ast->data)
		free(ast->data);
	if (ast->redirection_file)
		free(ast->redirection_file);
	free(ast);
}
