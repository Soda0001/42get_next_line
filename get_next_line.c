/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedeniz <sedeniz@student.42istanbul.com.t  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 15:07:02 by sedeniz           #+#    #+#             */
/*   Updated: 2026/08/16 17:21:24 by sedeniz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*call_gnl(int fd, char *buff, int start)
{
	ssize_t			bytes_read;
	static char		*storage;
	int				pos_nl;

	pos_nl = find_nl(storage, start);
	if (pos_nl >= 0)
		return (extract_line(&storage, pos_nl));
	start = find_end(storage);
	bytes_read = read(fd, buff, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buff[bytes_read] = '\0';
		storage = append_to_storage(storage, buff);
		if (!storage)
			return (NULL);
		pos_nl = find_nl(storage, start);
		if (pos_nl < 0)
			start += bytes_read;
		else
			return (extract_line(&storage, pos_nl));
		bytes_read = read(fd, buff, BUFFER_SIZE);
	}
	if (bytes_read == 0)
		return (handle_eof(&storage));
	return (NULL);
}

char	*get_next_line(int fd)
{
	char	*buff;
	char	*line;
	int		start;

	start = 0;
	if (!check_fd_isvalid(fd))
		return (NULL);
	buff = malloc(BUFFER_SIZE + 1);
	if (!buff)
		return (NULL);
	line = call_gnl(fd, buff, start);
	free(buff);
	return (line);
}
