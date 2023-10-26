/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:24:55 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/26 18:04:06 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/minishell.h"

void	init_echo(t_echo *e)
{
	e->i = 1;
	e->flag = 0;
	e->j = 0;
	status = 0;
}

void	echo_quote_bis(t_echo *e, char **arg)
{
	while (arg[e->i][e->j])
	{
		if (arg[e->i][e->j] != '\'')
		{
			e->flag = 0;
			ft_printf_fd(1, "%c", arg[e->i][e->j]);
		}
		else if (arg[e->i][e->j] == '\'' && e->flag == 0)
			e->flag = 1;
		e->j++;
	}
}

void	echo_quote(t_echo *e, char **arg)
{
	while (arg[e->i][e->j])
	{
		if (arg[e->i][e->j] != '"')
		{
			e->flag = 0;
			ft_printf_fd(1, "%c", arg[e->i][e->j]);
		}
		else if (arg[e->i][e->j] == '"' && e->flag == 0)
			e->flag = 1;
		e->j++;
	}
}

void	echo_quote_bis_bis(t_echo *e, char **arg)
{
	while (arg[e->i][e->j])
	{
		if (arg[e->i][e->j] != '"' && arg[e->i][e->j] != '\'')
		{
			e->flag = 0;
			ft_printf_fd(1, "%c", arg[e->i][e->j]);
		}
		else if (arg[e->i][e->j] == '\'' && e->flag == 0)
			e->flag = 1;
		e->j++;
	}
}
