/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 10:24:52 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/27 14:56:29 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/minishell.h"

void	hsigint(int sig)
{
	g_status = 128 + sig;
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	hsigint_exec(int sig)
{
	g_status = 128 + sig;
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	hsigint_hd(int sig)
{
	g_status = 128 + sig;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	hsigquit(int sig)
{
	g_status = 128 + sig;
	ft_printf_fd(2, "Quit (core dumped)\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}
