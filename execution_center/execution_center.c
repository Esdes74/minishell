/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_center.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 21:29:25 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/30 15:11:39 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"
#include "../incs/execution_center.h"

static int	initialise_exec_center(t_exec *ex, t_cmd *pip, t_list *spt);
static int	handle_builtins_parent(t_cmd *pip, t_exec *ex, t_list *spt);
static int	exec_children(pid_t *tab_pid, t_exec *ex, t_cmd *pip, t_list *spt);
static int	exec_wait(t_exec *ex, t_cmd *pip, pid_t *tab_pid, t_list *spt);

int	execution_center(t_list *spt, t_cmd *pip)
{
	int		ret;
	t_exec	ex;
	pid_t	*tab_pid;

	if (initialise_exec_center(&ex, pip, spt) == 1)
		return (1);
	ex.arg_count = counting_arg(pip->nb_proc, spt);
	if (ex.arg_count == NULL)
		return (1);
	ex.i = 0;
	ret = handle_builtins_parent(pip, &ex, spt);
	if (ret == 1)
		return (1);
	if (ret == 2)
		return (2);
	tab_pid = (pid_t *) malloc(sizeof(pid_t) * (pip->nb_proc));
	if (tab_pid == NULL)
	{
		close_all_pipes(pip);
		return (annihilation(spt, free, DEBUG), free(ex.arg_count), 1);
	}
	if (exec_wait(&ex, pip, tab_pid, spt) == 1)
		return (1);
	parent_wait_and_signal(&ex, tab_pid, pip);
	return (0);
}

static int	exec_wait(t_exec *ex, t_cmd *pip, pid_t *tab_pid, t_list *spt)
{
	while (ex->i < pip->nb_proc && pip->parent_builtin == FALSE && \
	ex->ret != 1)
	{
		g_status = 0;
		ex->id = fork();
		if (ex->id == 0)
		{
			if (exec_children(tab_pid, ex, pip, spt) == 1)
				return (1);
		}
		else
			tab_pid[ex->i] = ex->id;
		ex->i++;
	}
	return (0);
}

static int	exec_children(pid_t *tab_pid, t_exec *ex, t_cmd *pip, t_list *spt)
{
	free(tab_pid);
	ex->exec_cmd = string_for_cmd_center(ex->arg_count, ex->i, spt);
	free(ex->arg_count);
	annihilation(spt, free, DEBUG);
	if (ex->exec_cmd == NULL)
		return (error(MALLOC, NULL), 1);
	ex->exec_cmd = check_redirection(ex->exec_cmd, pip);
	if (ex->exec_cmd == NULL)
		return (close_all_pipes(pip), 1);
	if (pip->nb_proc > 1)
	{
		if (ex->i > 0 && pip->in == FALSE)
			if (dup2(pip->pipe[ex->i - 1][0], STDIN_FILENO) == -1)
				return (1);
		if (ex->i < pip->nb_pipe && pip->out == FALSE)
			if (dup2(pip->pipe[ex->i][1], STDOUT_FILENO) == -1)
				return (1);
		close_all_pipes(pip);
	}
	if (pip->here_pipe)
		if (pip->flag != 1)
			free(pip->here_pipe);
	if (cmd_center_simple(ex->exec_cmd, pip) == 1)
		return (anihilation(ex->exec_cmd), 1);
	return (0);
}

static int	handle_builtins_parent(t_cmd *pip, t_exec *ex, t_list *spt)
{
	int	i;

	i = 0;
	if (pip->nb_pipe == 0)
	{
		ex->exec_cmd = string_for_cmd_center(ex->arg_count, i, spt);
		if (ex->exec_cmd == NULL)
			return (error(MALLOC, NULL), 1);
		ex->buf = check_redirection_parent(ex->exec_cmd, pip);
		if (pip->flag == 1 && ex->buf == NULL)
			return (free(ex->arg_count), 2);
		if (ex->buf == NULL)
			return (free(ex->arg_count), annihilation(spt, free, DEBUG), 1);
		free(pip->here_pipe);
		while (ex->buf[i])
		{
			ex->exec_cmd = trash_quote(ex->buf);
			i++;
		}
		ex->value_ret = parent_builtins(pip, ex->buf, ex->exec_cmd);
		if (ex->value_ret == -1)
			return (annihilation(spt, free, DEBUG), free(ex->arg_count), \
			anihilation(ex->buf), 1);
		anihilation(ex->buf);
		pip->ind_hd = -1;
	}
	return (0);
}

static int	initialise_exec_center(t_exec *ex, t_cmd *pip, t_list *spt)
{
	unset_signals();
	ex->id = -1;
	ex->ret = 0;
	pip->parent_builtin = FALSE;
	pip->builtin = FALSE;
	pip->nb_proc = checking_pipe(spt);
	pip->nb_pipe = pip->nb_proc - 1;
	if (pip->nb_proc > 1)
		if (prep_pipe(pip) == 1)
			return (1);
	hd_signals();
	pip->hd_history = prep_hd(pip, spt);
	if (pip->status_hd == 2)
		return (1);
	unset_signals();
	return (0);
}
