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
static void	test(int sig);

void	signals(void)
{
	signal(CON_C, handle_control_c);
	signal(5, test);
}

static void	handle_control_c(int sig)
{
	(void) sig;

	printf("\nc'est la fin\n");
	kill(0, SIGKILL);
	/* exit(0); */
}

static void	test(int sig)
{
	(void) sig;

	printf("ceci est un test\n");
}
