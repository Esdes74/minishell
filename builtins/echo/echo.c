/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:01:38 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/30 15:17:25 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"
#include "../../incs/echo.h"

static int	dollar_check(char *arg);
// static void	second_echo(t_echo *e, char **arg);

void	echo(char **arg, int option)
{
	t_echo	e;
	// int		j;

	init_echo(&e);
	if (option == 1)
		e.i = position_echo_n(arg, &e.j, &e.flag, &option);
	while (arg[e.i])
	{
		// j = 0;
		if (dollar_check(arg[e.i]) == 0)
			ft_printf("%s", arg[e.i]);
		// else
		// {
			// while (arg[e.i][j])
			// {
			// 	if (arg[e.i][j] != '$')
			// 		ft_printf("%c", arg[e.i][j]);
			// 	j++;
			// }
		// }
		e.i += 1;
		if (arg[e.i])
			ft_printf(" ");
	}
	// e.type_flag = 0;
	// while (arg[e.i])
	// {
	// 	if (e.flag != 2)
	// 		e.j = 1;
	// 	if (e.flag == 2)
	// 		e.flag = 0;
	// 	while (arg[e.i][e.j] && arg[e.i][e.j] != '"' && arg[e.i][e.j] != '\'')
	// 		e.j++;
	// 	if ((arg[e.i][e.j] == '"' || arg[e.i][e.j] == '\'') 
	// 	&& arg[e.i][e.j + 1] != '\0')
	// 		echo_bis(&e, arg);
	// 	else
	// 		second_echo(&e, arg);
	// 	e.i++;
	// }
	if (option == 0)
		ft_printf_fd(1, "\n");
}

static int	dollar_check(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '$')
			return (1);
		i++;
	}
	return (0);
}
// {
// 	if (arg[e->i][0] == '"')
// 	{
// 		e->type_flag = 1;
// 		e->j = 1;
// 	}
// 	else if (arg[e->i][0] == '\'')
// 	{
// 		e->type_flag = 2;
// 		e->j = 1;
// 	}
// 	else
// 		e->j = 0;
// 	if (e->type_flag == 1)
// 		echo_quote(e, arg);
// 	else if (e->type_flag == 2)
// 		echo_quote_bis(e, arg);
// 	else
// 		echo_quote_bis_bis(e, arg);
// }

// static void	second_echo(t_echo *e, char **arg)
// {
// 	e->flag = 0;
// 	arg[e->i][e->j] = '\0';
// 	if (arg[e->i][0] == '"' || arg[e->i][0] == '\'')
// 		ft_printf_fd(1, &arg[e->i][1]);
// 	else
// 		ft_printf_fd(1, arg[e->i]);
// 	if ((arg[e->i][0] == '"' || arg[e->i][0] == '\'') && e->flag == 0)
// 	{
// 		e->flag = 1;
// 		e->j = 0;
// 		while (arg[e->i][e->j] != '\0' && arg[e->i][e->j] != '\n')
// 			e->j++;
// 	}
// 	else if ((arg[e->i][0] == '"' || arg[e->i][0] == '\'') && e->flag == 1)
// 		e->flag = 0;
// 	if (arg[e->i + 1] && e->flag == 0)
// 		ft_printf_fd(1, " ");
// }
