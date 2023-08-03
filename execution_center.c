/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_center.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 21:29:25 by dbaule            #+#    #+#             */
/*   Updated: 2023/08/03 15:34:48 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  checking_pipe(t_cell *cell, t_list *spt);
static int	prep_pipe(t_cmd *pip, int count);
static int  dup_in_cmd(t_cmd *pip, int i);
static int dup_out_cmd(t_cmd *pip, int i);

int execution_center(t_list *spt, char **env, t_cmd *pip)
{
    int     *arg_count;
    t_cell  *cell; // liste -> main_exec -> pipes, forks, dups, cmd_center
    char    **exec_cmd;
    int     i;
    int     id;
    int     count;

    i = 0;
    cell = malloc(sizeof(t_cell));
    count = checking_pipe(cell, spt);
    if (count > 1)
        if (prep_pipe(pip, count) == 1)
            return (1);
    arg_count = counting_arg(cell, count, spt);
    if (arg_count == NULL)
        return (1);
    while (i < count)
    {
        id = fork();
        if (id == 0)
        {
            add_list(getpid(), list);
            exec_cmd = string_for_cmd_center(arg_count, cell, i, spt);
            if (exec_cmd == NULL)
                return (1);
            // ft_printf_fd(2, "valeur de exec cmd %s\n", exec_cmd[i]);
            if (count > 1)
            {
                if (i > 0)
                    dup_in_cmd(pip, i - 1);
                if (i < pip->nb_pipe)
                    dup_out_cmd(pip, i);
                close_all_pipes(pip);
            }
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

static int	prep_pipe(t_cmd *pip, int count)
{
	int	i;

	i = 0;
    pip->nb_pipe = count - 1;
    pip->pipe = (int **) ft_calloc(pip->nb_pipe, sizeof(int *));
    if (pip->pipe == NULL)
    {
        return (1);
    }
	while (i < pip->nb_pipe)
	{
		pip->pipe[i] = (int *) ft_calloc(2, sizeof(int));
		if (pip->pipe[i] == NULL)
		{
			pip->nb_pipe = i;
			ft_printf_fd(2, "Error : Problem with calloc function\n");
			return (close_all_pipes(pip), 1);
		}
		if (pipe(pip->pipe[i]) == -1)
		{
			pip->nb_pipe = i;
			return (close_all_pipes(pip), error(PIPE, NULL), 1);
		}
		i++;
	}
	return (0);
}

static int  dup_in_cmd(t_cmd *pip, int i)
{
    (void)i;
    dup2(pip->pipe[0][0], STDIN_FILENO);
    return (0);
}

static int dup_out_cmd(t_cmd *pip, int i)
{
    (void)i;
    dup2(pip->pipe[0][1], STDOUT_FILENO);
    return (0);
}
