/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_center.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 21:29:25 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/25 15:24:41 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  checking_pipe(t_list *spt);
static int	prep_pipe(t_cmd *pip);
static int  dup_in_cmd(t_cmd *pip, int i);
static int dup_out_cmd(t_cmd *pip, int i);
// static int search_parent_builtins(char **tmp);

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
    // int     value_ret;
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
            return (-2);
    hd_signals();
    pip->hd_history = prep_hd(pip, spt);
    if (pip->status_hd == 1)
        return (2);
    unset_signals();
    arg_count = counting_arg(pip->nb_proc, spt);
    if (arg_count == NULL)
        return (1);
    if (pip->nb_pipe == 0)
    {
        tmp = spt->head;
        if (tmp == NULL)
            return (0);
        exec_cmd = string_for_cmd_center(arg_count, i, spt);
        if (exec_cmd == NULL)
            return (error(MALLOC, NULL), 1);
        buf = check_redirection_parent(exec_cmd, pip);
        anihilation(exec_cmd);
        if (buf == NULL)
            return (free(arg_count), 2); //anihilation(exec_cmd),
        if (parent_builtins(pip, spt, buf) == -1)
            return (annihilation(spt, free, DEBUG), free(arg_count), 1);
        anihilation(buf);
        exec_cmd = NULL;
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

// static int search_parent_builtins(char **tmp)
// {
//     char    *str;
//     int     i;

//     // while (tmp[i] && (tmp[i][0] == '<' || tmp[i][0] == '>')) // ne gere pas si la redirection est espace: "< test"
//     //     i++;
//     // if (tmp[i] == NULL)
//     //     return (0);
//     i = 0;
//     str = tmp[i];
//     if (ft_strlen(str) == 4 && ft_strncmp(str, "exit", 4) == 0)
//         return (1);
//     else if (ft_strlen(str) == 2 && ft_strncmp(str, "cd", 2) == 0)
//         return (2);
//     else if (ft_strlen(str) == 6 && ft_strncmp(str, "export", 6) == 0)
//     {
//         if (tmp[i + 1] == NULL)
//             return (0);
//         return (3);
//     }
//     else if (ft_strlen(str) == 5 && ft_strncmp(str, "unset", 5) == 0)
//         return (4);
//     return (0);
// }
