/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 19:27:29 by mleibeng          #+#    #+#             */
/*   Updated: 2024/02/22 07:19:37 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void create_hash(t_data *data, size_t init_size)
{
	t_hash *hashmap;

	hashmap = malloc(sizeof(t_hash));
	// handle malloc error
	hashmap->table = ft_calloc(init_size, sizeof(t_key));
	// handle malloc error
	hashmap->hash_size = 0;
	hashmap->hash_cap = init_size;
	data->var_hash = hashmap;
}

int	assign_hash_pos(char *key, size_t hash_cap)
{
	size_t	pos;
	size_t	i;

	pos = 0;
	i = 0;
	while (key[i])
	{
		pos = pos * 31 + key[i];
		i++;
	}
	return (pos % hash_cap);
}

int	resize_hash(t_hash *hash)
{
	int i;
	size_t	new_cap;
	size_t	new_pos;
	t_key	*new_map;

	i = 0;
	new_cap = hash->hash_cap * HASH_MULT;
	new_map = ft_calloc(new_cap, sizeof(t_key));
	if (!new_map)
		return (-1);
	while (i < hash->hash_cap)
	{
		if (hash->table[i].key)
		{
			new_pos = assign_hash_pos(hash->table[i].key, new_cap);
			while (new_map[new_pos].key)
				new_pos = (new_pos + 1) % new_cap;
			new_map[new_pos] = hash->table[i];
		}
	}
	free(hash->table);
	hash->table = new_map;
	hash->hash_cap = new_cap;
	return (0);
}

int insert_hash(t_hash *hash, char *key, char *value)
{
	size_t key_pos;
	if (hash->hash_size / hash->hash_cap > 0.7)
	{
		resize_hash(hash);
	}
	key_pos = assign_hash_pos(key, hash->hash_cap);
	while (hash->table[key_pos].key)
	{
		if (ft_strncmp(hash->table[key_pos].key, key, ft_strlen(key)) == 0)
		{
			free(hash->table[key_pos].value);
			hash->table[key_pos].value = ft_strdup(value + 1);
			return (0);
		}
		key_pos = (key_pos + 1) % hash->hash_cap;
	}
	hash->table[key_pos].key = ft_strdup(key);
	// printf("%s\n", hash->table[key_pos].key);
	hash->table[key_pos].value = ft_strdup(value);
	// printf("%s\n", hash->table[key_pos].value);
	hash->hash_size += 1;
	return (0);
}

void	create_key(t_data *data, char *occ)
{
	int		i;
	size_t	key_length;
	size_t	value_length;
	char	*key;
	char	*value;

	i = 0;
	key_length = occ - data->tokens[0].value;
	value_length = ft_strlen(occ + 1);
	key = malloc(key_length);
	value = malloc(value_length);
	ft_strlcpy(key, data->tokens[0].value, key_length + 1);
	ft_strlcpy(value, occ + 1, value_length + 1);
	insert_hash(data->var_hash, key, value);
	free(key);
	free(value);
}
