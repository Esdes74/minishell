/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_center.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 21:29:25 by dbaule            #+#    #+#             */
/*   Updated: 2023/08/02 00:57:31 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int checking_pipe(t_cell *cell, t_list *spt);

int execution_center(t_list *spt, char **env)
{
    int     count;
    int     *arg_count;
    t_cell  *cell; // liste -> main_exec -> pipes, forks, dups, cmd_center
    char    **exec_cmd;
    int     i;

    cell = spt->head;
    count = checking_pipe(cell, spt);
    cell = spt->head;
    arg_count = counting_arg(cell, count, spt);
    if (arg_count == NULL)
        return (1);

    i = 0; // il permet plus tard quand on mettre string_for_cmd_center dans une boucle, à repérer lenombre d'argument qu'il nous faut ! (je n'ai pas pris en compte le pipe !!!)
    
    exec_cmd = string_for_cmd_center(arg_count, cell, i, spt);
    if (exec_cmd == NULL)
        return (1);
    cmd_center_simple(exec_cmd, env);
    return (0);
}

static int checking_pipe(t_cell *cell, t_list *spt)
{
    int count;

    count = 1;
    while (cell != NULL)
    {
        if (((char *)(cell->data_cell->data))[0] == '|')
            count++;
        cell = cell->next;
    }
    cell = spt->head;
    return (count);
}
