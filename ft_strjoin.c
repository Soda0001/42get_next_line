/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedeniz <sedeniz@student.42istanbul.com.t  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 15:49:13 by sedeniz           #+#    #+#             */
/*   Updated: 2026/08/16 15:53:07 by sedeniz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	copy_string(char const *str, char const *to_copy)
{
	char	*result;
	char	*to_be_copied;
	int		i;

	result = (char *)str;
	to_be_copied = (char *)to_copy;
	i = 0;
	while (to_be_copied[i])
	{
		result[i] = to_be_copied[i];
		i++;
	}
	result[i] = '\0';
}

/* Edited to handle storage pointing at NULL at the start of GNL */
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*final_str;
	int		s1_len;
	int		s2_len;

	if (!s2)
		return (NULL);
	if (s1 == NULL)
		s1 = "";
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	final_str = malloc(s1_len + s2_len + 1);
	if (!final_str)
		return (NULL);
	copy_string(final_str, s1);
	copy_string(&final_str[s1_len], s2);
	return (final_str);
}
