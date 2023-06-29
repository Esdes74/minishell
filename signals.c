/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 16:50:47 by eslamber          #+#    #+#             */
/*   Updated: 2023/06/27 17:05:52 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_control_c(int sig);
static void	kill_all(int sig);

void	signals(void)
{
	signal(CON_C, handle_control_c);
	signal(KILL_ALL, kill_all);
	signal(SCAN, scan_all);
}

static void	handle_control_c(int sig)
{
	(void) sig;

	printf("\ndébut du scan des processus enfants pour tous les fermer\n");
	kill((int) list->head->data_cell->data, SCAN);
}

static void	kill_all(int sig)
{
	// TODO : Mettre en place l'algo de destruction des différentes 
	// structures
	// Faire une boucle pour trouver a partir de quand on doit envoyer
	// le signal dans la liste puis détruire les structures et envoyer 
	// le signal afin de pouvoir fermer le process en cours
	(void) sig;
	kill(KILL_ALL);
	annihilation(list, free, DEBUG);
	printf("exited\n");
	exit(0);
}

static void	scan_all(int sig)
{
	(void) sig;

	// TODO : faire une boucle pour commencer a scanner la liste du 
	// premier process et envoyer le signal kill_all a tous les process
	// enfants du process 1
}
