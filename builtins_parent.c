/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 10:31:07 by dbaule            #+#    #+#             */
/*   Updated: 2023/10/25 17:44:44 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  unset_env(t_cmd *pip, char *name_value, int len);
static int  unset_exp_env(t_cmd *pip, char *name_value, int len);

unsigned char    intermediate_exit(char **tmp)
{
    char    *new_tmp;
    long long     ret_value;
    int     i;
    char    *buf;
    int     len;

    len = 0;
    while (tmp[len])
        len++;
    if (len == 1)
        return (exitt(0));
    else
    {
        buf = (char *) tmp[1];
        new_tmp = check_quote(buf);
        if (new_tmp != tmp[1])
            free(tmp[1]);
        tmp[1] = new_tmp;
        i = 0;
        while (new_tmp[i])
        {
            if (!ft_isdigit(new_tmp[i]))
                if (i > 0 || (i == 0 && new_tmp[i] != '-' && new_tmp[i] != '+'))
                    return (error(NUM_ARG, "0"), exitt(2));
            i++;
        }
        ret_value = ft_atoi(new_tmp);
        if (ret_value == 0 && ft_strlen(buf) > 2 && check_zero(buf) == 0)
            return (ft_printf_fd(2, "Error : %s numeric argument required", buf), exitt(2));
    }
    if (len > 2)
        return(error(TOO_MANY_ARG, "exit"), exitt(1));
    return (exitt((unsigned char) ret_value));
}

unsigned char    exitt(unsigned char ret_value)
{
    // ft_printf_fd(1, "exit\n");
    // free_all(pip);
    status = ret_value;
    // silent_quit();
    return(ret_value);
}

void    cd(char **path, t_cmd *pip)
{
    int i;

    i = 0;
    while (path[i]) // faut adapter avec les redirection ??
    {
        if (i > 2)
            free(path[i]);
        i++;
    }
    if (i <= 2 && path != NULL && chdir(path[1]) != 0)
    {
        status = 1;
        ft_printf_fd(2, "-bash: cd: %s: No such file or directory\n", path[1]);
        free(path[1]);
        return ;
    }
    if (i > 2)
    {
        error(TOO_MANY_ARG, "cd");
        free(path[1]);
        free(path[2]);
        return ;
    }
    status = 0;
    pip->builtin = TRUE;
}

int unset(t_cmd *pip, char *name_value)
{
    int     i;
    int     trigger;

    i = 0;
    trigger = 0;
    if (name_value[0] == '\0' || ft_isdigit(name_value[0]) == 1)
        return (free(name_value), ft_printf_fd(2, "Error : not a valid identifier\n"), status = 1, 0);
    while (name_value[i] && (ft_isalnum(name_value[i]) == 1 || name_value[i] == '_'))
        i++;
    if (name_value[i] != '\0')
        return (free(name_value), ft_printf_fd(2, "Error : not a valid identifier\n"), status = 1, 0);
    i = 0;
    while (pip->env[i])
    {
        if (ft_strncmp(pip->env[i], name_value, count_name_env(pip->env[i]) - 1) == 0\
        && (count_name_env(pip->env[i]) == count_name_env(name_value)))
            trigger++;
        i++;
    }
    i = 0;
    while (pip->exp_env[i])
    {
        if (ft_strncmp(pip->exp_env[i] + 11, name_value, \
        count_name_env(pip->exp_env[i] + 11) - 1) == 0 && (count_name_env(pip->exp_env[i] + 11) == count_name_env(name_value)))
            trigger++;
        i++;
    }
    if (trigger == 0)
    {
        pip->builtin = TRUE;
        return (free(name_value), 0);
    }
    else
        if (unset_env(pip, name_value, i) == 1)
            return (free(name_value), 1);
    if (unset_exp_env(pip, name_value, i) == 1)
        return (free(name_value), 1);
    free(name_value);
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
        if (strncmp(pip->env[j], name_value, count_name_env(pip->env[j]) - 1) == 0 && (count_name_env(pip->env[j]) == count_name_env(name_value)))
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
            count_name_env(pip->exp_env[j] + 11) - 1) == 0 && (count_name_env(pip->exp_env[j] + 11) == count_name_env(name_value)))
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
