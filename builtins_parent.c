/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 10:31:07 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/20 01:03:03 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  unset_env(t_cmd *pip, char *name_value, int len);
static int  unset_exp_env(t_cmd *pip, char *name_value, int len);

unsigned char    intermediate_exit(t_list *tmp)
{
    char    *new_tmp;
    int     ret_value;
    int     i;

    if (tmp->len > 2)
        return(error(TOO_MANY_ARG, "exit"), exitt(1));
    if (tmp->len == 1)
        return (exitt(0));
    else if (tmp->len == 2)
    {
        new_tmp = check_quote((char *) tmp->head->next->data_cell->data);
        if (new_tmp != tmp->head->next->data_cell->data)
            free(tmp->head->next->data_cell->data);
        tmp->head->next->data_cell->data = new_tmp;
        i = 0;
        while (new_tmp[i])
        {
            if (!ft_isdigit(new_tmp[i]))
                if (i > 0 || (i == 0 && new_tmp[i] != '-' && new_tmp[i] != '+'))
                    return (error(NUM_ARG, "0"), exitt(2));
            i++;
        }
        ret_value = ft_atoi(new_tmp);
    }
    return (exitt((unsigned char) ret_value));
}

unsigned char    exitt(unsigned char ret_value)
{
    // ft_printf_fd(1, "exit\n");
    // free_all(pip);
    status = (int)ret_value;
    // silent_quit();
    return(ret_value);
}

void    cd(char *path, t_cmd *pip, t_list *spt)
{
    if (spt->len <= 2 && path != NULL && chdir(path) != 0)
    {
        ft_printf_fd(2, "-bash: cd: %s: No such file or directory\n");
        return ;
    }
    else if (spt->len > 2)
    {
        error(TOO_MANY_ARG, "cd");
        return ;
    }
    pip->builtin = TRUE;
}

int unset(t_cmd *pip, char *name_value)
{
    int     i;
    int     trigger;

    i = 0;
    trigger = 0;
    while (pip->env[i])
        if (ft_strncmp(pip->env[i++], name_value, (ft_strlen(name_value))) == 0)
            trigger++;
    i = 0;
    while (pip->exp_env[i])
        if (ft_strncmp(pip->exp_env[i++] + 11, name_value, \
        (ft_strlen(name_value))) == 0)
            trigger++;
    if (trigger == 0)
    {
        pip->builtin = TRUE;
        return (0);
    }
    else
        if (unset_env(pip, name_value, i) == 1)
            return (1);
    if (unset_exp_env(pip, name_value, i) == 1)
        return (1);
    return (0);
}

static int  unset_env(t_cmd *pip, char *name_value, int len)
{
    int     i;
    int     j;
    char    **new_env;

    i = 0;
    j = 0;
    new_env = (char **) malloc(sizeof(char *) * (len + 1));
    if (new_env == NULL)
        return (error(MALLOC, "0"), 1);
    while (pip->env[j])
    {
        if (strncmp(pip->env[j], name_value, (ft_strlen(name_value))) == 0 && ft_in('=', name_value) == 1)
            free(pip->env[j++]);
        else if (strncmp(pip->env[j], name_value, (ft_strlen(name_value) + 1)) == 0 && ft_in('=', name_value) == 0)
            free(pip->env[j++]);
        else
            new_env[i++] = pip->env[j++];
    }
    new_env[i] = NULL;
    free(pip->env);
    pip->env = new_env;
    pip->builtin = TRUE;
    return (0);
}

static int  unset_exp_env(t_cmd *pip, char *name_value, int len)
{
    int     i;
    int     j;
    char    **new_env;

    i = 0;
    j = 0;
    new_env = (char **) malloc(sizeof(char *) * (len + 1));
    if (new_env == NULL)
        return (error(MALLOC, "0"), 1);
    while (pip->exp_env[j])
    {
        if (strncmp(pip->exp_env[j] + 11, name_value, \
            (ft_strlen(name_value))) == 0)
            free(pip->exp_env[j++]);
        else
            new_env[i++] = pip->exp_env[j++];
    }
    new_env[i] = NULL;
    free(pip->exp_env);
    pip->exp_env = new_env;
    pip->builtin = TRUE;
    return (0);
}
