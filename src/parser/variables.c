/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 05:27:01 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/25 11:24:03 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "minishell.h"
#include "parser.h"
#include "utilities.h"

char	*ft_get_variable(char **input)
{
	char	*var;
	int		len;
	int		i;

	len = 1;
	while ((*input)[len] && (ft_isalnum((*input)[len]) || (*input)[len] == '_'))
		len++;
	var = ft_calloc(len + 1, sizeof(char));
	if (!var)
		return (NULL);
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

char	*expand_variable(char *word, char **new_word, int *word_len,
		char **envp)
{
	char	*var_end;
	char	*var_name;
	char	*var_value;

	var_end = word + 1;
	while (*var_end && (ft_isalnum(*var_end) || *var_end == '_'))
		var_end++;
	if (var_end == word + 1)
		*new_word = ft_append_char(*new_word, word_len, *word);
	else
	{
		var_name = ft_substr(word + 1, 0, var_end - word - 1);
		var_value = ft_getenv(var_name, envp);
		free(var_name);
		if (var_value && *var_value)
			*new_word = ft_append_str(*new_word, word_len, var_value);
		else
		{
			free(*new_word);
			return (ft_strdup(""));
		}
	}
	return (var_end);
}

char	*handle_variable_expansion(char *word, char **envp)
{
	char	*new_word;
	int		word_len;

	new_word = NULL;
	word_len = 0;
	while (*word)
	{
		if (*word == '$' && *(word + 1) != '?' && !ft_isspace(*(word + 1)))
			word = expand_variable(word, &new_word, &word_len, envp);
		else
		{
			new_word = ft_append_char(new_word, &word_len, *word);
			word++;
		}
	}
	new_word = ft_append_char(new_word, &word_len, '\0');
	return (new_word);
}

char	*ft_get_word(char **input)
{
	char	*word;
	int		len;
	int		i;

	len = 0;
	word = NULL;
	while ((*input)[len] && !ft_isspace((*input)[len]) && (*input)[len] != '\''
		&& (*input)[len] != '"' && (*input)[len] != '$' && (*input)[len] != '|'
		&& (*input)[len] != '>' && (*input)[len] != '<')
		len++;
	word = ft_calloc(len + 1, sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = (*input)[i];
		i++;
	}
	word[i] = '\0';
	*input += len;
	return (word);
}
