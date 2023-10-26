/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 09:45:13 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/26 12:02:41 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*ft_strjoin_pip(char *dest, char *src)
// {
// 	size_t	len_tot;
// 	char	*new;

// 	len_tot = ft_strlen(dest) + ft_strlen(src) + 1;
// 	new = (char *) malloc(sizeof(char) * len_tot);
// 	if (new == NULL)
// 		return (NULL);
// 	new[0] = '\0';
// 	ft_strlcat(new, dest, len_tot);
// 	if (dest)
// 		free(dest);
// 	ft_strlcat(new, src, len_tot);
// 	return (new);
// }

int	ft_is_redirection(const char *str, int i)
{
	if (str[i] == '>' && i == 0)
		return (str[i] == '>' || (str[i] == '>' && str[i + 1] == '>'));
	else if (str[i] == '>' && i > 0)
		return ((str[i] == '>' || (str[i] == '>' && str[i + 1] == '>')) \
		&& str[i - 1] != '>');
	else if (str[i] == '<' && i > 0)
		return ((str[i] == '<' || (str[i] == '<' && str[i + 1] == '<')) \
		&& str[i - 1] != '<');
	return (str[i] == '<' || (str[i] == '<' && str[i + 1] == '<'));
}

char	*rm_char_exp(char *str, char c)
{
	size_t	i;
	size_t	z;
	char	*new_str;

	i = 0;
	z = 0;
	while (str[i])
	{
		if (str[i] == c)
			z++;
		i++;
	}
	new_str = malloc(sizeof(char) * (i - z + 1));
	if (!new_str)
		return (error(MALLOC, "0"), NULL);
	i = 0;
	z = 0;
	while (str[i])
	{
		if (str[i] != c)
			new_str[z++] = str[i];
		i++;
	}
	new_str[z] = '\0';
	return (new_str);
}

void silent_quit(void)
{
	exit(0);
}
