/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:22:37 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/20 12:17:09 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

char	*ft_append_char(char *str, int *len, char c)
{
	char	*new_str;
	int		new_len;

	new_len = *len + 1;
	new_str = malloc(new_len + 1);
	if (!new_str)
		return (NULL);
	if (str)
	{
		ft_memcpy(new_str, str, *len);
		free(str);
	}
	new_str[*len] = c;
	new_str[new_len] = '\0';
	*len = new_len;
	return (new_str);
}

char	*ft_append_str(char *str1, int *len1, const char *str2)
{
	char	*new_str;
	int		len2;
	int		new_len;

	len2 = ft_strlen(str2);
	new_len = *len1 + len2;
	new_str = malloc(new_len + 1);
	if (!new_str)
		return (NULL);
	if (str1)
	{
		ft_memcpy(new_str, str1, *len1);
		free(str1);
	}
	ft_memcpy(new_str + *len1, str2, len2);
	new_str[new_len] = '\0';
	*len1 = new_len;
	return (new_str);
}

void	ast_append(t_ast *parent, t_ast *child)
{
	t_ast	*current;

	if (!parent || !child)
		return ;
	if (!parent->left)
	{
		parent->left = child;
		return ;
	}
	current = parent->left;
	while (current->right)
		current = current->right;
	current->right = child;
}

t_ast	*create_ast_node(t_ast_type type, char *data)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	node->type = type;
	node->data = data;
	node->redirection_file = NULL;
	node->redirection_mode = 0;
	node->left = NULL;
	node->right = NULL;
	node->redirections = NULL;
	node->num_redirections = 0;
	return (node);
}
