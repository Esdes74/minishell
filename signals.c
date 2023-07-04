/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 16:50:47 by eslamber          #+#    #+#             */
/*   Updated: 2023/07/04 17:32:25 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_control_c(int sig);
static void	kill_all(int sig);
static void	scan_all(int sig);

void	signals(void)
{
	signal(CON_C, handle_control_c);
	signal(KILL_ALL, kill_all);
	signal(SCAN, scan_all);
}

void	quit(void)
{
	printf("exit\n");
	scan_all(CON_C);
	annihilation(list, free, DEBUG);
	exit(0);
}

static void	handle_control_c(int sig)
{
	(void) sig;

	printf("\ndébut du scan des processus enfants pour tous les fermer\n");
	kill(*((pid_t *) list->head->data_cell->data), SCAN);
	/* scan_all(SCAN); */
}

static void	kill_all(int sig)
{
	(void)	sig;
	t_cell	*tmp;

	printf("Fonction KILL\n");
	tmp = list->head;
	while (tmp != NULL && *((pid_t *) tmp->data_cell->data) != getpid())
		tmp = tmp->next;
	if (tmp != NULL)
		tmp = tmp->next;
	while (tmp != NULL)
	{
		kill(*((pid_t *) tmp->data_cell->data), KILL_ALL);
		tmp = tmp->next;
	}
	annihilation(list, free, DEBUG);
	printf("exited\n");
	exit(0);
}

static void	scan_all(int sig)
{
	(void)	sig;
	t_cell	*tmp;
	int		i;

	printf("Fonction SCAN\n");
	if (list->len > 1)
	{
		i = 0;
		while (list->len > 0)
		{
			tmp = untail_list(list, DEBUG);
			if (i != 0)
				kill(*((pid_t *) tmp->data_cell->data), KILL_ALL);
			free(tmp->data_cell->data);
			free(tmp->data_cell);
			free(tmp);
			i++;
		}
		if (add_list(getpid(), list) == 1)
			return (error(ADD_LIST));
	}
}
