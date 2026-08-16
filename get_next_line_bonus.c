/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedeniz <sedeniz@student.42istanbul.com.t  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 21:01:26 by sedeniz           #+#    #+#             */
/*   Updated: 2026/08/16 21:16:00 by sedeniz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*call_gnl(int fd, char *buff, int start)
{
	ssize_t		bytes_read;
	static char	*storage[MAX_FD];
	int			pos_nl;

	pos_nl = find_nl(storage[fd], start);
	if (pos_nl >= 0)
		return (extract_line(&storage[fd], pos_nl));
	start = find_end(storage[fd]);
	bytes_read = read(fd, buff, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buff[bytes_read] = '\0';
		storage[fd] = append_to_storage(storage[fd], buff);
		if (!storage[fd])
			return (NULL);
		pos_nl = find_nl(storage[fd], start);
		if (pos_nl < 0)
			start += bytes_read;
		else
			return (extract_line(&storage[fd], pos_nl));
		bytes_read = read(fd, buff, BUFFER_SIZE);
	}
	if (bytes_read == 0)
		return (handle_eof(&storage[fd]));
	return (NULL);
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
