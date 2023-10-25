/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 16:50:47 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/25 15:25:58 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	main_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = &hsigint;
    sigaction(CON_C, &sa, NULL);
	sa.sa_handler = IGN;
	sigaction(QUIT, &sa, NULL);
}

void	exec_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = &hsigint_exec;
    sigaction(CON_C, &sa, NULL);
	sa.sa_handler = &hsigquit;
    sigaction(QUIT, &sa, NULL);
}

void	hd_signals()
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = &hsigint_hd;
    sigaction(CON_C, &sa, NULL);
}

void	unset_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = IGN;
    sigaction(CON_C, &sa, NULL);
    sigaction(QUIT, &sa, NULL);
}

void	silent_quit(void)
{
	exit(0);
}

void	hsigint(int sig)
{
	status = 128 + sig;
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
    rl_redisplay();
}

void	hsigint_exec(int sig)
{
	status = 128 + sig;
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	hsigint_hd(int sig)
{
	status = 128 + sig;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	hsigquit(int sig)
{
	status = 128 + sig;
	ft_printf_fd(2, "Quit (core dumped)\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}
