/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 20:59:43 by cpuiu             #+#    #+#             */
/*   Updated: 2024/04/23 22:58:50 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"
#include "executor.h"
#include "minishell.h"
#include "redirection.h"

#define RANDOM_BYTES 12

static int	read_random_bytes(unsigned char *buffer, size_t size)
{
	int		fd;
	size_t	result;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return (-1);
	result = read(fd, buffer, size);
	close(fd);
	if (result == size)
		return (0);
	return (-1);
}

static void	append_hex_string(char *dest, unsigned char *bytes,
		size_t num_bytes)
{
	size_t	offset;
	size_t	i;

	i = 0;
	offset = ft_strlen(dest);
	while (i < num_bytes)
	{
		ft_snprintf(dest + offset, 3, "%d", bytes[i]);
		offset += 2;
		i++;
	}
	dest[offset] = '\0';
}

static char	*generate_tmp_file_name(void)
{
	static const char	tmp_dir[] = "/tmp/minishell_";
	const size_t		tmp_dir_len = sizeof(tmp_dir) - 1;
	const size_t		filename_buffer_size = tmp_dir_len + 25;
	char				*tmp_file;
	unsigned char		random_bytes[RANDOM_BYTES];

	tmp_file = malloc(filename_buffer_size);
	if (!tmp_file)
		return (NULL);
	ft_strcpy(tmp_file, tmp_dir);
	if (read_random_bytes(random_bytes, RANDOM_BYTES) != 0)
	{
		free(tmp_file);
		return (NULL);
	}
	append_hex_string(tmp_file, random_bytes, RANDOM_BYTES);
	return (tmp_file);
}

int	create_temp_file(char **tmp_file)
{
	int	fd;

	*tmp_file = generate_tmp_file_name();
	fd = open(*tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_error("minishell", *tmp_file, strerror(errno));
		free(*tmp_file);
		*tmp_file = NULL;
		return (-1);
	}
	return (fd);
}

int	execute_file_redirection_list(t_ast **file_node_ptr)
{
	int		result;
	t_ast	*current_node;

	result = 0;
	current_node = *file_node_ptr;
	while (current_node && current_node->redirection_file != NULL)
	{
		result = execute_file_redirection(current_node);
		if (result != 0)
			return (result);
		current_node = current_node->right;
	}
	return (0);
}
