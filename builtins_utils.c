/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:36:54 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/20 05:42:39 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    print_export(char **exp_env)
{
    int i;
    int j;
    int flag;

    i = 0;
    while (exp_env[i])
    {
        j = 0;
        flag = 0;
        while (exp_env[i][j])
        {
            ft_printf("%c", exp_env[i][j++]);
            if (exp_env[i][j - 1] == '=' && flag++ == 0)
                ft_printf("\"");
        }
        if (flag != 0)
            ft_printf("\"");
        ft_printf("\n");
        i++;
    }
}

char	*rm_char(char *str, char c)
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
	free(str);
	return (new_str);
}

int ft_strncmp_wo_plus(char *first, char *sec, size_t n)
{
	size_t			len;
	unsigned char	*u_first;
	unsigned char	*u_sec;

	len = 0;
	u_first = (unsigned char *) first;
	u_sec = (unsigned char *) sec;
	while ((u_first[len] != '\0' || u_sec[len] != '\0') && len < n)
	{
		if (u_sec[len] != '+')
		{
			if (u_first[len] != u_sec[len])
				return (u_first[len] - u_sec[len]);
		}
		len++;
	}
	return (0);
}

int	position_echo_n(char **arg, int *j, int *flag, int *option)
{
	int i;
	int k;
	
	i = 1;
	k = 0;
	while (arg[i])
	{
		if (arg[i][k] != '-')
		{
			return (i);	
		}
		while (arg[i][k])
		{
			if (arg[i][k + 1] && arg[i][k] == '-' && arg[i][k + 1] == 'n' && *flag != 2)
			{
				*j = k;
				k += 1;
				while (arg[i][k] && arg[i][k] == 'n')
					k += 1;
			}
			if (arg[i][k])
			{
				*flag = 2;
				*j = k;
				*option = 0;
				return (i);
			}
		}
		k = 0;
		i++;
	}
	return(i);
}

int	count_name_env(char *str)
{
	int i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}
