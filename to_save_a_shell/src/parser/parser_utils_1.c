/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 14:12:13 by cpuiu             #+#    #+#             */
/*   Updated: 2024/04/06 14:12:57 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utilities.h"

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

int	ast_count_nodes(t_ast *ast)
{
	int	count;

	count = 1;
	if (ast == NULL)
		return (0);
	count += ast_count_nodes(ast->left);
	count += ast_count_nodes(ast->right);
	return (count);
}

char	*ft_get_variable(char **input)
{
	char	*var;
	int		len;
	int		i;

	len = 1;
	while ((*input)[len] && (ft_isalnum((*input)[len]) || (*input)[len] == '_'))
		len++;
	var = ft_calloc(len + 1, sizeof(char));
	var[0] = '$';
	i = 1;
	while (i < len)
	{
		var[i] = (*input)[i];
		i++;
	}
	var[i] = '\0';
	*input += len;
	return (var);
}
