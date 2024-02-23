/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:59:39 by mleibeng          #+#    #+#             */
/*   Updated: 2024/02/22 07:43:53 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	safetycheck(char *string)
{
	if (!string)
	{
		perror("Error with string handling");
		return (1);
	}
	return (0);
}

void	path_command_check(t_token *token)
{
	size_t	length;
	char	*save_path;
	char	*path;
	char	*directory;
	char	*full_path;

	path = getenv("PATH");
	if (safetycheck(path))
		return ;
	save_path = ft_strdup(path);
	if (safetycheck(save_path))
		return ;
	if (token && token->value != NULL)
	{
		directory = ft_strtok(save_path, ":");
		while (directory)
		{
			length = ft_strlen(directory) + ft_strlen(token->value) + 2;
			full_path = malloc(length);
			if (safetycheck(full_path))
			{
				free(save_path);
				return ;
			}
			ft_strlcpy(full_path, directory, ft_strlen(directory) + 1);
			full_path[ft_strlen(directory)] = '/';
			ft_strlcpy(full_path + ft_strlen(directory) + 1, token->value,
				ft_strlen(token->value) + 1);
			if (!access(full_path, X_OK))
			{
				token->path_command = full_path;
				free(save_path);
				return ;
			}
			free(full_path);
			directory = ft_strtok(NULL, ":");
		}
	}
	free(save_path);
	token->path_command = NULL;
}

void	check_commands(t_data *data)
{
	size_t	i;

	i = -1;
	while (++i < data->num_of_elements)
	{
		if (data->tokens[i].pos == 0)
		{
			if (!ft_strncmp(data->tokens[i].value, "export", 7)
				|| !ft_strncmp(data->tokens[i].value, "unset", 6)
				|| !ft_strncmp(data->tokens[i].value, "env", 4)
				|| !ft_strncmp(data->tokens[i].value, "exit", 5)
				|| !ft_strncmp(data->tokens[i].value, "echo", 5)
				|| !ft_strncmp(data->tokens[i].value, "cd", 3)
				|| !ft_strncmp(data->tokens[i].value, "pwd", 4))
				data->tokens[i].type = COMMAND;
			else
			{
				path_command_check(&data->tokens[i]);
				if (data->tokens[i].path_command != NULL)
					data->tokens[i].type = COMMAND;
			}
		}
	}
}

void	check_specs(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->num_of_elements)
	{
		if (!ft_strncmp(data->tokens[i].value, "|", 1)
		|| !ft_strncmp(data->tokens[i].value, ">", 1)
		|| !ft_strncmp(data->tokens[i].value, ">>", 4)
		|| !ft_strncmp(data->tokens[i].value, "<", 5)
		|| !ft_strncmp(data->tokens[i].value, "<<", 5))
			data->tokens[i].type = SPEC;
		else if (!ft_strncmp(data->tokens[i].value, "$?", 3))
			data->tokens[i].type = EDGE;
		i++;
	}
}

void	remove_quote(t_token *tokens)
{
	char	*move;

	move = tokens->value;
	while (*(move + 1))
	{
		*move = *(move + 1);
		move++;
	}
	*move = '\0';
}

void	check_quoted_strings(t_data *data)
{
	size_t	i;

	i = -1;
	while (++i < data->num_of_elements)
	{
		if (data->tokens[i].value[0] == '\'')
		{
			data->tokens[i].s_quoted = true;
			remove_quote(&data->tokens[i]);
		}
		else if (data->tokens[i].value[0] == '"')
		{
			data->tokens[i].d_quoted = true;
			remove_quote(&data->tokens[i]);
		}
	}
}

char	*retrieve_var(t_hash *hash, char *key)
{
	size_t	pos;
	size_t	start;

	pos = assign_hash_pos(key, hash->hash_cap);
	start = pos;
		while(hash->table[pos].key != NULL)
		{
			if(!ft_strncmp(hash->table[pos].key, key, ft_strlen(key)))
				return (hash->table[pos].value);
			pos = pos + 1;
			pos = pos % hash->hash_cap;
			if (pos == start)
				break ;
		}
		return NULL;
}

// /*Causes a seg-fault*/
void	change_token(t_data *data, size_t i)
{
	size_t	token_len;
	size_t	var_len;
	size_t	new_token_len;
	char	*new_token;

	token_len = ft_strlen(data->tokens[i].value);
	var_len = ft_strlen(data->hash_data->var);
	new_token_len = token_len - data->hash_data->key_len + var_len;
	printf(" tok_len:%zu var_len:%zu new_tok_len:%zu key_len:%zu\n", token_len, var_len, new_token_len, data->hash_data->key_len);
	new_token = malloc(new_token_len + 1);
	if (new_token)
	{
		ft_strlcpy(new_token, data->tokens[i].value, data->hash_data->key_len - token_len + 1);
		printf("%s\n", new_token);
		ft_strlcat(new_token, data->hash_data->var, new_token_len + 1);
		printf("%s\n", new_token);
		ft_strlcat(new_token, data->hash_data->end, new_token_len + 1);
		printf("%s\n", new_token);
		data->tokens[i].value = new_token;
		printf("%s\n", data->tokens[i].value);
	}
}

/*Causes a seg-fault*/
void	expand_vars(t_data *data)
{
	// size_t	i;

	// i = -1;
	// while (++i < data->num_of_elements)
	// {
		data->hash_data->start = ft_strchr(data->tokens[0].value, '$');
		printf("%s", data->tokens[0].value);
		while (data->hash_data->start)
		{
			data->hash_data->end = data->hash_data->start + 1;
			while (*data->hash_data->end != '\0' && ft_isalnum(*data->hash_data->end))
				data->hash_data->end++;
			data->hash_data->key_len = data->hash_data->end - data->hash_data->start - 1;
			if (data->hash_data->key_len > 0)
			{
				data->hash_data->val = malloc(data->hash_data->key_len + 1);
				ft_strlcpy(data->hash_data->val, data->hash_data->start + 1, data->hash_data->key_len + 1);
				// printf("%s\n", data->hash_data->val);
				data->hash_data->var = retrieve_var(data->var_hash, data->hash_data->val);
				// printf("%s\n", data->hash_data->var);
				free(data->hash_data->val);
				if (data->hash_data->var)
					change_token(data, 0);
			}
			data->hash_data->start = ft_strchr(data->hash_data->end, '$');
		}
	}
	// search_env();
// }

void	check_vars(t_data *data)
{
	size_t	i;
	char	*occ;

	i = -1;
	occ = NULL;
	while (++i < data->num_of_elements)
	{
		occ = ft_strchr(data->tokens[i].value, '=');
		if (occ != NULL && data->num_of_elements == 1)
		{
			create_key(data, occ);
			return ;
		}
	}
	expand_vars(data);
}

void	type_the_tokens(t_data *data)
{
	check_commands(data);
	check_specs(data);
	check_quoted_strings(data);
	check_vars(data); //expand the content with the value of the variable
}

void	check_pipe(char **split_array, t_data *data)
{
	int	i;
	int	count_pos;

	count_pos = 0;
	i = 0;
	while (split_array[i])
	{
		if (!ft_strncmp(split_array[i], "|", 1))
			count_pos = 0;
		else
		{
			data->tokens[i].pos = count_pos;
			count_pos++;
		}
		i++;
	}
}

void	init_tokens(char **split_array, t_data *data)
{
	int	i;

	i = 0;
	while (split_array[i])
	{
		data->tokens[i].type = ARG;
		data->tokens[i].path_command = NULL;
		data->tokens[i].pos = -2;
		data->tokens[i].value = split_array[i];
		data->tokens[i].s_quoted = false;
		data->tokens[i].d_quoted = false;
		i++;
	}
}

// just for debugging
const char	*type_to_string(t_type type)
{
	switch (type)
	{
	case COMMAND:
		return ("COMMAND");
	case ARG:
		return ("ARGUMENT");
	case SPEC:
		return ("SPECIAL_CHAR");
	case VAR:
		return ("VARIABLE");
	default:
		return ("UNKNOWN");
	}
}

// void error_check_tokens(t_data *data)
// {
// 	//when for example i do not find a COMMAND in the input,
// 	//i will just clear the elements and return to the readline and wait for the next input
// 	// another example if i do not find an option to the command that is being used then i also return to readline
// 	// also if the pipe doesnt work, or another special char
// 	// also if the arguments cant execute <---- this one maybe has to be check during execution, i think the functions throw errors by themselves
// 	// etc...etc..etc... whenever i can find a mistake basically here i can catch it maybe
// }

void	parse_tokens(char **split_array, t_data *data)
{
	size_t	i;

	i = 0;
	while (split_array[i])
		i++;
	data->num_of_elements = i;
	data->tokens = (t_token *)malloc(sizeof(t_token) * (data->num_of_elements));
	init_tokens(split_array, data);
	check_pipe(split_array, data);
	type_the_tokens(data);
	// error_check_tokens(data);
	i = 0;
	/* debug text
	while (i < data->num_of_elements)
	{
		printf("%s\n", data->tokens[i].path_command);
		if (!ft_strncmp(data->tokens[i].value, "|", 1))
			data->tokens[i].pos = -1;
		printf("%s %d %s %d %d\n", data->tokens[i].value, data->tokens[i].pos,
			type_to_string(data->tokens[i].type), data->tokens[i].d_quoted,
			data->tokens[i].s_quoted);
		i++;
	}
	*/
}
