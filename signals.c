/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 16:50:47 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/25 11:20:01 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_control_c(int sig);
// static void	kill_all(int sig);
// static void	scan_all(int sig);

void	signals(void)
{
	struct sigaction sa;
    
	sa.sa_handler = handle_control_c;
    sigaction(CON_C, &sa, NULL);
}

void	silent_quit(void)
{
	// scan_all(CON_C);
	exit(0);
}

static void	handle_control_c(int sig)
{
	(void)	sig;

	rl_replace_line("", 0); // Effacer la ligne en cours
    rl_point = 0; // Déplacer le curseur au début de la ligne
    rl_redisplay(); // Afficher le prompt

}

