/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedeniz <sedeniz@student.42istanbul.com.t  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 15:06:45 by sedeniz           #+#    #+#             */
/*   Updated: 2026/08/16 16:38:18 by sedeniz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd);

int	check_fd_isvalid(int fd)
{
	if (fd < 0)
		return (0);
	return (1);
}

char	*append_to_storage(char *storage, char *buff)
{
	char	*temp;

	temp = ft_strjoin(storage, buff);
	if (!temp)
		return (NULL);
	free(storage);
	storage = temp;
	return (storage);
}

int	find_nl(char *str, int start)
{
	int	i;

	if (!str)
		return (-1);
	if (!start)
		start = 0;
	else
		i = start;
	while (str[i] && str[i] != '\n')
	{
		i++;
	}
	if (str[i] == '\0')
		return (-1);
	else
		return (i);
}

int	is_include_nl(char *str, int start)
{
	int	i;

	i = start;
	if (!str)
		return (0);
	while (str[i] && str[i] != '\n')
	{
		i++;
	}
	if (str[i] == '\0')
		return (0);
	else
		return (1);
}
