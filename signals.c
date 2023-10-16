/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 16:50:47 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/16 14:46:16 by eslamber         ###   ########.fr       */
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
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(CON_C, &sa, NULL);
	// signal(CON_C, handle_control_c);
	// signal(KILL_ALL, kill_all);
	// signal(SCAN, scan_all);
}

void	silent_quit(void)
{
	// scan_all(CON_C);
	exit(0);
}

static void	handle_control_c(int sig)
{
	(void)	sig;
	// char	*buf;
	// char	*buff;

	rl_replace_line("", 0); // Effacer la ligne en cours
    rl_point = 0; // Déplacer le curseur au début de la ligne
    rl_redisplay(); // Afficher le prompt
	// kill(*((pid_t *) list->head->data_cell->data), SCAN);
	// buf = getcwd(NULL, 0);
	// if (buf == NULL)
	// 	exitt();
	// buff = ft_strjoin(buf, "$ ");
	// free(buf);
	// if (buff == NULL)
	// 	exitt();
	// // ft_printf_fd(2, buff);
	// free(buff);
}

// static void	kill_all(int sig)
// {
// 	(void)	sig;
// 	t_cell	*tmp;

// 	tmp = list->head;
// 	while (tmp != NULL && *((pid_t *) tmp->data_cell->data) != getpid())
// 		tmp = tmp->next;
// 	if (tmp != NULL)
// 		tmp = tmp->next;
// 	while (tmp != NULL)
// 	{
// 		kill(*((pid_t *) tmp->data_cell->data), KILL_ALL);
// 		tmp = tmp->next;
// 	}
// 	annihilation(list, free, DEBUG);
// 	exit(0);
// }

// static void	scan_all(int sig)
// {
// 	(void)	sig;
// 	t_cell	*tmp;
// 	int		i;

// 	if (list->len > 1)
// 	{
// 		i = 0;
// 		while (list->len > 0)
// 		{
// 			tmp = unstack_list(list, DEBUG);
// 			if (i != 0)
// 				kill(*((pid_t *) tmp->data_cell->data), KILL_ALL);
// 			free(tmp->data_cell->data);
// 			free(tmp->data_cell);
// 			free(tmp);
// 			i++;
// 		}
// 		if (add_list(getpid(), list) == 1)
// 			return (error(ADD_LIST, NULL));
// 	}
// }
