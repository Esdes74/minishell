/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_center.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 21:29:25 by dbaule            #+#    #+#             */
/*   Updated: 2023/08/02 15:10:04 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  checking_pipe(t_cell *cell, t_list *spt);
// static int	prep_pipe(t_cmd *pip, int count);

int execution_center(t_list *spt, char **env)
{
    int     *arg_count;
    t_cell  *cell; // liste -> main_exec -> pipes, forks, dups, cmd_center
    char    **exec_cmd;
    int     i;
    // t_cmd   *pip;
    int     id;
    int     count;

    cell = malloc(sizeof(t_cell));
    // pip = malloc(sizeof(t_cmd));
    count = checking_pipe(cell, spt);
    // if (prep_pipe(pip, count) == 1)
    //     return (1);
    arg_count = counting_arg(cell, count, spt);
    if (arg_count == NULL)
        return (1);
    i = 0; // il permet plus tard quand on mettre string_for_cmd_center dans une boucle, à repérer lenombre d'argument qu'il nous faut ! (je n'ai pas pris en compte le pipe !!!)
    while (i < count)
    {
        id = fork();
        if (id == 0)
        {
            add_list(getpid(), list);
            // close_all_pipes(pip);
            exec_cmd = string_for_cmd_center(arg_count, cell, i, spt);
            if (exec_cmd == NULL)
                return (1);
            cmd_center_simple(exec_cmd, env);
        }
        add_list(id, list);
        i++;
    }
    while (list->len > 1)
        rmtail_list(list, TRUE, DEBUG);
    return (0);
}

static int checking_pipe(t_cell *cell, t_list *spt)
{
    int count;

    count = 1;
    cell = spt->head;
    while (cell != NULL)
    {
        
        if (((char *)(cell->data_cell->data))[0] == '|')
            count++;
        cell = cell->next;
    }
    cell = spt->head;
    return (count);
}

// static int	prep_pipe(t_cmd *pip, int count)
// {
// 	int	i;

// 	i = 0;
//     pip->pipe = (int **) ft_calloc(count, sizeof(int *));
// 	while (i < count)
// 	{
// 		pip->pipe[i] = (int *) ft_calloc(2, sizeof(int));
// 		if (pip->pipe[i] == NULL)
// 		{
// 			pip->nb_pipe = i;
// 			ft_printf_fd(2, "Error : Problem with calloc function\n");
// 			return (close_all_pipes(pip), 1);
// 		}
// 		if (pipe(pip->pipe[i]) == -1)
// 		{
// 			pip->nb_pipe = i;
// 			return (close_all_pipes(pip), error(PIPE, NULL), 1);
// 		}
// 		i++;
// 	}
//     pip->nb_pipe = count; 
// 	return (0);
// }
