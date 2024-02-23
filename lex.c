/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:59:49 by mleibeng          #+#    #+#             */
/*   Updated: 2024/02/22 05:48:12 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_in(const char *input_line, t_data *data)
{
	char	**split_array;
	char	*trimcheck;

	data->input = readline(input_line);
	if (data->input == NULL)
		return (EOF);
	trimcheck = ft_strtrim(data->input, DELIMITERS);
	if (ft_strncmp(data->input, "\n", ft_strlen(data->input)) != 0
		&& ft_strncmp(data->input, "", 1) != 0
		&& trimcheck)
	{
		add_history(data->input);
		free(trimcheck);
	}
	split_array = ft_split_quotes(data->input, DELIMITERS);
	/* debug code
	// int i = 0;
	// while (split_array[i])
	// {
	// 	printf("%s\n", split_array[i]);
	// 	i++;
	*/
	parse_tokens(split_array, data);
	return (0);
}

// t_type	check_type(char *token)
// {
// 	int i;

// 	i = 0;
// 	// if (!strncmp(token, BUILTIN))
// 	//	THEN: return COMMAND;
// 	// if !strncmp token, other type of envp command
// 	//	THEN: return COMMAND;
// 	// if !strcmp token, special character like pipes etc..asm
// 	//	THEN return SPEC;
// 	// if !strcmp token, variable expansions like $() or $?
// 	//	THEN return VAR;
// 	// else
// 	//	THEN return ARG;
// }

// void	free_splitarr(char **tokens)
// {
// 	int i;

// 	i = -1;
// 	while (tokens[++i])
// 		free(tokens[i]);
// 	free(tokens);
// }

// void	tokenize(t_data *data)
// {
// 	char **tokens;
// 	t_tokenlist *current;
// 	t_tokenlist *new_token;
// 	int	i;

// 	i = -1;
// 	tokens = ft_split_delims(data->input, " \t\n");
// 	current = NULL;
// 	while (tokens[++i])
// 	{
// 		new_token = (t_tokenlist *)malloc(sizeof(t_tokenlist));
// 		// new_token->token.type = check_type(tokens[i]);
// 		new_token->token.value = ft_strdup(tokens[i]);
// 		if (!current)
// 			data->tokens = new_token;
// 		else
// 			current->next = new_token;
// 		current = new_token;
// 	}
// 	free_splitarr(tokens);
// }

// // This stuff is for checking if EOF is sent by ctrl-d
// // or if exit is called somewhere,
// // so i can add the history and call the exit function
// /*	if (!input || strncmp (data->input, "exit", 6))
// 	{
// 		add_history(data->input);
// 		exit_func(data);
// 	}
// */




	// split_array = NULL;
	// i = 0;
	// s_quote_flag = 0;
	// d_quote_flag = 0;
	// while (data->input[i])
	// {
	// 	if (data->input[i] == 34)
	// 		if (check_for_second_dquote(data->input) != -1)
	// 		{
	// 			split_array = ft_split_quotes(data->input, DELIMITERS);
	// 			break ;
	// 		}
	// 	if (data->input[i] == 39)
	// 		if (check_for_second_dquote(data->input) != -1)
	// 		{
	// 			split_array = ft_split_quotes(data->input, DELIMITERS);
	// 			break ;
	// 		}
	// 	i++;
	// }