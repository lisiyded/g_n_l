/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spowers <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:16:30 by spowers           #+#    #+#             */
/*   Updated: 2020/07/15 11:07:35 by spowers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char			*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}

void			ft_free(char **str)
{
	if (str != NULL && *str != NULL)
		free(*str);
	*str = NULL;
}

static int		get_line(char **str_static, char **line)
{
	int			i;
	char		*tmp_line;

	i = 0;
	while ((*str_static)[i] != '\n' && (*str_static)[i] != '\0')
		i++;
	if ((*str_static)[i] == '\n')
	{
		*line = ft_substr(*str_static, 0, i);
		tmp_line = ft_strdup(&(*str_static)[i + 1]);
		ft_free(str_static);
		*str_static = tmp_line;
		if ((*str_static)[0] == '\0')
			ft_free(str_static);
	}
	else
	{
		*line = ft_strdup(*str_static);
		ft_free(str_static);
	}
	return (1);
}

static int		ft_return(char **str_static, char **line, int fd, int b_read)
{
	if (b_read < 0)
		return (-1);
	if (b_read == 0 && str_static[fd] == NULL)
		return (0);
	return (get_line(&str_static[fd], line));
}

int				get_next_line(int fd, char **line)
{
	char		buf[BUFFER_SIZE + 1];
	int			b_read;
	static char	*str_static[FD_MAX];
	char		*str_tmp;

	if (fd < 0 || !line || BUFFER_SIZE <= 0)
		return (-1);
	while ((b_read = read(fd, &buf, BUFFER_SIZE)) > 0)
	{
		buf[b_read] = '\0';
		if (str_static[fd] == NULL)
			str_static[fd] = ft_strdup(buf);
		else
		{
			str_tmp = ft_strjoin(str_static[fd], buf);
			ft_free(&str_static[fd]);
			str_static[fd] = str_tmp;
		}
		if (ft_strchr(str_static[fd], '\n'))
			break ;
	}
	return (ft_return(str_static, line, fd, b_read));
}
