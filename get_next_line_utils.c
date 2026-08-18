/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedeniz <sedeniz@student.42istanbul.com.t  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 15:06:45 by sedeniz           #+#    #+#             */
/*   Updated: 2026/08/16 22:50:17 by sedeniz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*extract_line(char **storage, int pos_nl)
{
	char	*remaining_storage;
	char	*line;

	line = ft_substr(*storage, 0, pos_nl + 1);
	remaining_storage = ft_strdup(*storage + pos_nl + 1);
	free(*storage);
	*storage = remaining_storage;
	return (line);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	s_len;
	size_t	i;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	i = 0;
	if (start >= s_len)
		return (ft_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

int	find_nl(char *str, int start)
{
	int	i;

	if (!str)
		return (-1);
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

/* Edited to handle storage pointing at NULL at the start of GNL */
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*final_str;
	int		i;

	if (!s2)
		return (NULL);
	if (s1 == NULL)
		s1 = "";
	final_str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!final_str)
		return (NULL);
		i = 0;
	while (s1[i])
	{
		final_str[i] = s1[i];
		i++;
	}
	i = 0;
	while (s2[i])
	{
		final_str[ft_strlen(s1) + i] = s2[i];
		i++;
	}
	final_str[ft_strlen(s1) + ft_strlen(s2)] = '\0';
	return (final_str);
}