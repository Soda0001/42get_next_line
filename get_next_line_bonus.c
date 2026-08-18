/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedeniz <sedeniz@student.42istanbul.com.tr +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 21:01:26 by sedeniz           #+#    #+#             */
/*   Updated: 2026/08/18 12:17:56 by sedeniz          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	ft_strlen(const char *s)
{
	size_t	letter_count;

	if (!s)
		return (0);
	letter_count = 0;
	while (s[letter_count])
		letter_count++;
	return (letter_count);
}

char	*ft_strdup(const char *src)
{
	char	*dup;
	int		i;

	i = 0;
	if (!src)
		return (NULL);
	while (src[i])
		i++;
	dup = malloc(sizeof(char) * (i + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (src[i])
	{
		dup[i] = src[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*read_until_nl(char **storage, char *buff, int fd, int start)
{
	ssize_t	bytes_read;
	char	*temp;
	int		pos_nl;

	bytes_read = read(fd, buff, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buff[bytes_read] = '\0';
		temp = ft_strjoin(*storage, buff);
		free(*storage);
		*storage = temp;
		if (!*storage)
			return (NULL);
		pos_nl = find_nl(*storage, start);
		if (pos_nl >= 0)
			return (extract_line(storage, pos_nl));
		start += bytes_read;
		bytes_read = read(fd, buff, BUFFER_SIZE);
	}
	if (bytes_read == 0)
	{
		temp = *storage;
		*storage = NULL;
		return (temp);
	}
	return (NULL);
}

char	*call_gnl(int fd, char *buff, int start)
{
	static char	*storage[MAX_FD];
	int			pos_nl;

	pos_nl = find_nl(storage[fd], start);
	if (pos_nl >= 0)
		return (extract_line(&storage[fd], pos_nl));
	start = ft_strlen(storage[fd]);
	return (read_until_nl(&storage[fd], buff, fd, start));
}

char	*get_next_line(int fd)
{
	char	*buff;
	char	*line;
	int		start;

	start = 0;
	if (fd < 0 || fd >= MAX_FD)
		return (NULL);
	buff = malloc(BUFFER_SIZE + 1);
	if (!buff)
		return (NULL);
	line = call_gnl(fd, buff, start);
	free(buff);
	return (line);
}
