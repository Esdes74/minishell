/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_center_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 21:01:08 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/28 21:06:54 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"
#include "../incs/execution_center.h"

void	parent_wait_and_signal(t_exec *ex, pid_t *tab_pid, t_cmd *pip)
{
	if (ex->id != 0)
		free(ex->arg_count);
	if (pip->nb_pipe > 0)
		close_all_pipes(pip);
	ex->i = pip->parent_builtin;
	while (ex->i < pip->nb_proc)
	{
		waitpid(tab_pid[ex->i], &ex->statut, 0);
		ex->i++;
	}
	free(tab_pid);
	if (pip->parent_builtin == FALSE && pip->builtin == FALSE && ex->ret != 1)
	{
		if (WIFSIGNALED(ex->statut))
		{
			if (WTERMSIG(ex->statut) == SIGINT)
				hsigint_exec(SIGINT);
			else if (WTERMSIG(ex->statut) == SIGQUIT)
				hsigquit(SIGQUIT);
			ex->exit_status = 128 + WTERMSIG(ex->statut);
		}
		else if (WIFEXITED(ex->statut))
			ex->exit_status = WEXITSTATUS(ex->statut);
		g_status = ex->exit_status;
	}
}
