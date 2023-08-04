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
static int	prep_pipe(t_cmd *pip, int count);
static int  dup_in_cmd(t_cmd *pip, int i);
static int dup_out_cmd(t_cmd *pip, int i);

static void check_redirection(char **arg, t_bool *in, t_bool *out);
static void heredoc(char *arg);

int execution_center(t_list *spt, char **env, t_cmd *pip)
{
    int     *arg_count;
    char    **exec_cmd;
    int     i;
    int     id;
    int     count;
    t_bool  in;
    t_bool  out;

    i = 0;
    count = checking_pipe(spt);
    if (count > 1)
        if (prep_pipe(pip, count) == 1)
            return (1);
    arg_count = counting_arg(count, spt);
    if (arg_count == NULL)
        return (1);
    while (i < count)
    {
        id = fork();
        if (id == 0)
        {
            add_list(getpid(), list);
            exec_cmd = string_for_cmd_center(arg_count, i, spt);
            if (exec_cmd == NULL)
                return (1);
            check_redirection(exec_cmd, &in, &out);
            ft_printf_fd(2, "valeur de exec cmd %s\n", exec_cmd[i]);
            if (count > 1)
            {
                if (i > 0 && in == FALSE)
                    dup_in_cmd(pip, i - 1);
                if (i < pip->nb_pipe && out == FALSE)
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
    dup2(pip->pipe[i][0], STDIN_FILENO);
    return (0);
}

static int dup_out_cmd(t_cmd *pip, int i)
{
    dup2(pip->pipe[i][1], STDOUT_FILENO);
    return (0);
}

static void check_redirection(char **arg, t_bool *in, t_bool *out)
{
    char    **tmp;
    int     file;
    int     compt;
    int     i;
    int     j;

    i = 0;
    *in = FALSE;
    *out = FALSE;
    while (arg[i]) // récupération des différentes redirection
    {
        if (arg[i][0] == '<')
        {
            *in = TRUE;
            if (arg[i][1] == '\0')
            {
                if (arg[i + 1] == NULL)
                    return (error(TOKEN, "0"));
                file = open(arg[i + 1], O_RDONLY);
                if (file == -1)
                    return (error(OPEN, "0"));
                else if (dup2(file, STDIN_FILENO) == -1)
                    return (close(file), error(DUP, "0"));
                close(file);
                i++;
            }
            else if (arg[i][1] == '<' && arg[i][2] == '\0')
            {
                heredoc(arg[i + 1]);
                i++;
            }
            else if (arg[i][1] == '<' && arg[i][2] != '\0')
                heredoc(&arg[i][2]);
            else
            {
                file = open(&arg[i][1], O_RDONLY);
                if (file == -1)
                    return (error(OPEN, "0"));
                else if (dup2(file, STDIN_FILENO) == -1)
                    return (close(file), error(DUP, "0"));
                close(file);
            }
        }
        else if (arg[i][0] == '>')
        {
            *out = TRUE;
            if (arg[i][1] == '\0')
            {
                if (arg[i + 1] == NULL)
                    return (error(TOKEN, "0"));
                file = open(arg[i + 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
                if (file == -1)
                    return (error(OPEN, "0"));
                else if (dup2(file, STDOUT_FILENO) == -1)
                    return (close(file), error(DUP, "0"));
                close(file);
                i++;
            }
            else if (arg[i][1] == '>')
            {
                if (arg[i][2] == '\0')
                {
                    if (arg[i + 1] == NULL)
                        return (error(TOKEN, "0"));
                    file = open(arg[i + 1], O_CREAT | O_RDWR | O_APPEND, 0644);
                    if (file == -1)
                        return (error(OPEN, "0"));
                    else if (dup2(file, STDOUT_FILENO) == -1)
                        return (close(file), error(DUP, "0"));
                    close(file);
                    i++;
                }
                else
                {
                    file = open(&arg[i][2], O_CREAT | O_RDWR | O_APPEND, 0644);
                    if (file == -1)
                        return (error(OPEN, "0"));
                    else if (dup2(file, STDOUT_FILENO) == -1)
                        return (close(file), error(DUP, "0"));
                    close(file);
                }
            }
            else
            {
                file = open(&arg[i][1], O_CREAT | O_RDWR | O_TRUNC, 0644);
                if (file == -1)
                    return (error(OPEN, "0"));
                else if (dup2(file, STDOUT_FILENO) == -1)
                    return (close(file), error(DUP, "0"));
                close(file);
            }
        }
        i++;
    }
    if (*in == FALSE && *out == FALSE) // S'il n'y a pas de redirections pas besoin de retravailler le tableau de chaine de caractères
        return ;
    i = 0;
    compt = 0;
    while (arg[i]) // compte le nombre de redirection pour refaire un tableau de chaines de caractères
    {
        if (arg[i][0] == '<')
        {
            compt--;
            if (arg[i][1] == '\0' || (arg[i][1] == '<' && arg[i][2] == '\0'))
                i += 1;
        }
        else if (arg[i][0] == '>')
        {
            compt--;
            if (arg[i][1] == '\0' || (arg[i][1] == '>' && arg[i][2] == '\0'))
                i += 1;
        }
        i++;
        compt++;
    }
    tmp = (char **) malloc(sizeof(char *) * compt);
    i = 0;
    j = 0;
    while (i < compt) // ici on ajoute les chaines de caractère correctes a tmp, on free les redirections car elles ne servent plus et on remet tmp dans arg a la fin
    {
        if (arg[i][0] == '<')
        {
            if (arg[i][1] == '\0' || (arg[i][1] == '<' && arg[i][2] == '\0'))
            {
                free(arg[i + 1]);
                i++;
            }
            free(arg[i]);
        }
        else if (arg[i][0] == '>')
        {
            if (arg[i][1] == '\0' || (arg[i][1] == '>' && arg[i][2] == '\0'))
            {
                free(arg[i + 1]);
                i++;
            }
            free(arg[i]);
        }
        else
            tmp[j] = arg[i];
        i++;
        j++;
    }
    free(arg);
    arg = tmp;
    printf("j = %d\n", j);
    arg[j] = NULL;
}

static void heredoc(char *arg)
{
    (void) arg;
    return;
}