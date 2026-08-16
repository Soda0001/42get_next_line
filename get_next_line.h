/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedeniz <sedeniz@student.42istanbul.com.t  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 15:07:26 by sedeniz           #+#    #+#             */
/*   Updated: 2026/08/16 17:22:28 by sedeniz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>

int		check_fd_isvalid(int fd);
void	copy_to_stash(char *stash, char *buff, ssize_t bytes, int i);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s);
size_t	ft_strlen(const char *s);
char	*get_next_line(int fd);
char	*append_to_storage(char *storage, char *buff);
char	*extract_line(char **storage, int pos_nl);
char	*handle_eof(char **stash);
int		find_nl(char *str, int start);
int		find_end(char *str);

#endif
