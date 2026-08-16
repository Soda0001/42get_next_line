/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_end.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedeniz <sedeniz@student.42istanbul.com.t  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 16:02:39 by sedeniz           #+#    #+#             */
/*   Updated: 2026/08/16 16:03:00 by sedeniz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	find_end(char *str)
{
	int	end;

	end = 0;
	if (!str)
		return (0);
	while (str[end])
	{
		end++;
	}
	return (end);
}
