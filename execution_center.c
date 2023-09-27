/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_center.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 21:29:25 by dbaule            #+#    #+#             */
/*   Updated: 2023/08/03 17:30:00 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  checking_pipe(t_list *spt);
static int	prep_pipe(t_cmd *pip);
static int  dup_in_cmd(t_cmd *pip, int i);
static int dup_out_cmd(t_cmd *pip, int i);

int execution_center(t_list *spt, t_cmd *pip)
{
    char    **exec_cmd;
    int     *arg_count;
    int     status;
    int     exit_status;
    int     i;
    int     id;

    i = 0;
    id = -1;
    pip->parent_builtin = FALSE;
    pip->builtin = FALSE;
    pip->nb_proc = checking_pipe(spt);
    pip->nb_pipe = pip->nb_proc - 1;
    if (pip->nb_proc > 1)
        if (prep_pipe(pip) == 1)
            return (1);
    pip->hd_history = prep_hd(pip, spt);
    arg_count = counting_arg(pip->nb_proc, spt);
    if (arg_count == NULL)
        return (1);
    if (pip->nb_pipe == 0)
        if (search_parent_builtins(pip, spt) == -1)
            return (free(arg_count), -2);
    while (i < pip->nb_proc && pip->parent_builtin == FALSE)
    {
        id = fork();
        if (id == 0)
        {
            exec_cmd = string_for_cmd_center(arg_count, i, spt);
            free(arg_count);
            annihilation(spt, free, DEBUG);
            if (exec_cmd == NULL)
                return (error(MALLOC, NULL), 1);
            add_list(getpid(), list);
            exec_cmd = check_redirection(exec_cmd, pip);
            if (exec_cmd == NULL)
                return (-1);
            if (pip->nb_proc > 1)
            {
                if (i > 0 && pip->in == FALSE)
                    dup_in_cmd(pip, i - 1);
                if (i < pip->nb_pipe && pip->out == FALSE)
                    dup_out_cmd(pip, i);
                close_all_pipes(pip);
            }
            if (pip->here_pipe)
            {
                free(pip->here_pipe);
                anihilation(pip->hd_history);
            }
            if (cmd_center_simple(exec_cmd, pip) == 1)
                return (anihilation(exec_cmd), -1);
        }
        else
            add_list(id, list);
        i++;
    }
    if (id != 0)
        free(arg_count);
    i = pip->parent_builtin;
    if (pip->nb_pipe > 0)
    {
        close_all_pipes(pip);
    }


    // Récupération du code de sortie du programme
	while (i < pip->nb_proc + 1)
    {
        wait(&status);
        i++;
    }
    // Si je suis dans une éxécution de builtin alors je ne rentre pas dedans
    if (pip->builtin == FALSE)
    {
        if (WIFSIGNALED(status))
            exit_status = WTERMSIG(status);
        else if (WIFEXITED(status))
            exit_status = WEXITSTATUS(status);
        pip->status = exit_status; // Stockage du code de sortie
    }


    while (list->len > 1)
        rmtail_list(list, TRUE, DEBUG);
    return (0);
}

static int checking_pipe(t_list *spt)
{
    int count;
    t_cell *tmp;

    count = 1;
    tmp = spt->head;
    while (tmp != NULL)
    {
        if (((char *)(tmp->data_cell->data))[0] == '|')
            count++;
        tmp = tmp->next;
    }
    return (count);
}

static int	prep_pipe(t_cmd *pip)
{
	int	i;

	i = 0;
    pip->pipe = (int **) ft_calloc(pip->nb_pipe, sizeof(int *));
    if (pip->pipe == NULL)
    {
        return (1);
    }
	while (i < pip->nb_pipe)
	{
		pip->pipe[i] = (int *)ft_calloc(2, sizeof(int));
		if (pip->pipe[i] == NULL)
		{
			pip->nb_pipe = i;
			return (error(CALLOC, "0"), close_all_pipes(pip), 1);
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
    if (dup2(pip->pipe[i][0], STDIN_FILENO) == -1)
        return (1);
    return (0);
}

static int  dup_out_cmd(t_cmd *pip, int i)
{
    if (dup2(pip->pipe[i][1], STDOUT_FILENO) == -1)
        return (1);
    return (0);
}
