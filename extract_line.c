/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedeniz <sedeniz@student.42istanbul.com.t  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 16:01:58 by sedeniz           #+#    #+#             */
/*   Updated: 2026/08/16 17:05:21 by sedeniz          ###   ########.fr       */
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
