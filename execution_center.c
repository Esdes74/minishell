/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_center.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 21:29:25 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/27 14:36:20 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/minishell.h"

static int  checking_pipe(t_list *spt);
static int	prep_pipe(t_cmd *pip);

int execution_center(t_list *spt, t_cmd *pip)
{
    char    **exec_cmd;
    pid_t   *tab_pid;
    int     *arg_count;
    int     statut;
    int     exit_status;
    int     i;
    int     id;
    int     ret;
    int     value_ret;
    char    **buf;
    t_cell  *tmp;

    unset_signals();
    i = 0;
    id = -1;
    ret = 0;
    pip->parent_builtin = FALSE;
    pip->builtin = FALSE;
    pip->nb_proc = checking_pipe(spt);
    pip->nb_pipe = pip->nb_proc - 1;
    if (pip->nb_proc > 1)
        if (prep_pipe(pip) == 1)
            return (annihilation(spt, free, DEBUG), -1);
    hd_signals();
    pip->hd_history = prep_hd(pip, spt);
    if (pip->status_hd == 2 && pip->nb_proc > 1)
	{
        return (close_all_pipes(pip), annihilation(spt, free, DEBUG), 1);
	}
	if (pip->status_hd == 2)
		return (annihilation(spt, free, DEBUG), 1);
    unset_signals();
    arg_count = counting_arg(pip->nb_proc, spt);
    if (arg_count == NULL && pip->nb_proc > 1)
	{
        return (close_all_pipes(pip), annihilation(spt, free, DEBUG), 1);
	}
	if (arg_count == NULL)
		return (annihilation(spt, free, DEBUG), 1);
    if (pip->nb_pipe == 0)
    {
        tmp = spt->head;
        if (tmp == NULL)
            return (0);
        exec_cmd = string_for_cmd_center(arg_count, i, spt);
        if (exec_cmd == NULL)
            return (free(arg_count), annihilation(spt, free, DEBUG), 1);
        buf = check_redirection_parent(exec_cmd, pip);
        if (buf == NULL)
            return (free(arg_count), 1);
        value_ret = parent_builtins(pip, buf);
        if (value_ret == -1)
            return (annihilation(spt, free, DEBUG), free(arg_count), anihilation(buf), 1);
        anihilation(buf);
    }
    tab_pid = (pid_t *) malloc(sizeof(pid_t) * (pip->nb_proc));
    if (tab_pid == NULL)
        return (annihilation(spt, free, DEBUG), free(arg_count), 1);
    while (i < pip->nb_proc && pip->parent_builtin == FALSE && ret != 1)
    {
        status = 0;
        id = fork();
        if (id == 0)
        {
            free(tab_pid);
            exec_cmd = string_for_cmd_center(arg_count, i, spt);
            free(arg_count);
            annihilation(spt, free, DEBUG);
            if (exec_cmd == NULL)
                return (error(MALLOC, NULL), 1);
            exec_cmd = check_redirection(exec_cmd, pip);
            if (exec_cmd == NULL)
                return (-1);
            if (pip->nb_proc > 1)
            {
                if (i > 0 && pip->in == FALSE)
                    if (dup2(pip->pipe[i - 1][0], STDIN_FILENO) == -1)
        				return (1);
                if (i < pip->nb_pipe && pip->out == FALSE)
                    if (dup2(pip->pipe[i][1], STDOUT_FILENO) == -1)
        				return (1);
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
            tab_pid[i] = id;
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
	while (i < pip->nb_proc)
    {
        waitpid(tab_pid[i], &statut, 0);
        i++;
    }
    free(tab_pid);
    // Si je suis dans une éxécution de builtin alors je ne rentre pas dedans
    if (pip->parent_builtin == FALSE && pip->builtin == FALSE  && ret != 1) // && flag_status == 1
    {
        if (WIFSIGNALED(statut))
        {
            if (WTERMSIG(statut) == SIGINT)
                hsigint_exec(SIGINT);
            else if (WTERMSIG(statut) == SIGQUIT)
                hsigquit(SIGQUIT);
            exit_status = 128 + WTERMSIG(statut);
        }
        else if (WIFEXITED(statut))
            exit_status = WEXITSTATUS(statut);
        status = exit_status; // Stockage du code de sortie
    }
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
        return (error(MALLOC, "0"), 1);
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
			return (pip->nb_pipe = i, free(pip->pipe[i]), \
			close_all_pipes(pip), error(PIPE, "0"), 1);
		i++;
	}
	return (0);
}
