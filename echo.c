/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:01:38 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/26 18:01:46 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/minishell.h"

static void	echo_bis(t_echo *e, char **arg);
static void	second_echo(t_echo *e, char **arg);

void	echo(char **arg, int option)
{
	t_echo	e;

	init_echo(&e);
	if (option == 1)
		e.i = position_echo_n(arg, &e.j, &e.flag, &option);
	e.type_flag = 0;
	while (arg[e.i])
	{
		if (e.flag != 2)
			e.j = 1;
		if (e.flag == 2)
			e.flag = 0;
		while (arg[e.i][e.j] && arg[e.i][e.j] != '"' && arg[e.i][e.j] != '\'')
			e.j++;
		if ((arg[e.i][e.j] == '"' || arg[e.i][e.j] == '\'') \
		&& arg[e.i][e.j + 1] != '\0')
			echo_bis(&e, arg);
		else
			second_echo(&e, arg);
		e.i++;
	}
	if (option == 0)
		ft_printf_fd(1, "\n");
}

static void	echo_bis(t_echo *e, char **arg)
{
	if (arg[e->i][0] == '"')
	{
		e->type_flag = 1;
		e->j = 1;
	}
	else if (arg[e->i][0] == '\'')
	{
		e->type_flag = 2;
		e->j = 1;
	}
	else
		e->j = 0;
	if (e->type_flag == 1)
		echo_quote(e, arg);
	else if (e->type_flag == 2)
		echo_quote_bis(e, arg);
	else
		echo_quote_bis_bis(e, arg);
}

static void	second_echo(t_echo *e, char **arg)
{
	e->flag = 0;
	arg[e->i][e->j] = '\0';
	if (arg[e->i][0] == '"' || arg[e->i][0] == '\'')
		ft_printf_fd(1, &arg[e->i][1]);
	else
		ft_printf_fd(1, arg[e->i]);
	if ((arg[e->i][0] == '"' || arg[e->i][0] == '\'') && e->flag == 0)
	{
		e->flag = 1;
		e->j = 0;
		while (arg[e->i][e->j] != '\0' && arg[e->i][e->j] != '\n')
			e->j++;
	}
	else if ((arg[e->i][0] == '"' || arg[e->i][0] == '\'') && e->flag == 1)
		e->flag = 0;
	if (arg[e->i + 1] && e->flag == 0)
		ft_printf_fd(1, " ");
}
