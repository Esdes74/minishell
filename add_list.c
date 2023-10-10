/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 11:08:50 by eslamber          #+#    #+#             */
/*   Updated: 2023/07/03 11:08:54 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int add_list(pid_t data, t_list *list)
{
    pid_t *new;

    new = (pid_t *) malloc(sizeof(pid_t));
    if (new == NULL)
        return (annihilation(list, free, DEBUG), error(MALLOC, "0"), 1);
    *new = data;
    if (tailing_list(list, new, PID, DEBUG) == 0)
        return (annihilation(list, free, DEBUG), free(new), \
        error(TAILING, "0"), 1);
    return (0);
}
