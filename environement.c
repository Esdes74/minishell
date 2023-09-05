/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 14:26:08 by dbaule            #+#    #+#             */
/*   Updated: 2023/09/05 02:47:53 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
static int check_if_double(char **env, char *cmp);
static char **ft_cpy_env(char **env);

int cpy_env(char **env, t_cmd *pip)
{
    int i;
    
    i = 0;
    while (env[i])
        i++;
    pip->env = malloc(sizeof(char*) * (i + 1));
    if (pip->env == NULL)
        return (error(MALLOC, 0), 1);
    i = 0;
    while (env[i])
    {
        pip->env[i] = ft_strdup(env[i]);
        if (pip->env[i] == NULL)
            return (error(MALLOC, 0), free(pip->env), 1);
        i++;
    }
    pip->env[i] = NULL;
    return (0);
}

int initialize_exp_env(t_cmd *pip, char **env)
{
    int i;
    int z;
    char *buf;
    char **buf_env;
    
    i = 0;
    buf_env = ft_cpy_env(env);
    if (!buf_env)
        return (1);
    while (buf_env[i])
    {
        z = i + 1;
        while (buf_env[z])
        {
            if (ft_strncmp(buf_env[i], buf_env[z], ft_strlen(buf_env[i])) > 1)
            {
                if (check_if_double(buf_env, buf_env[z]) == 0)
                {
                    buf = buf_env[i];
                    buf_env[i] = buf_env[z];
                    buf_env[z] = buf;
                }
            }
            z++;
        }
        i++;
    }
    buf_env[i] = NULL;
    pip->exp_env = ft_calloc(sizeof (char*), (i + 1));
    if (!pip->exp_env)
        return (1);
    i = 0;
    if (strncmp(buf_env[i], "declare -x ", 11) != 0)
    {
        while (buf_env[i])
        {
            z = 11 + ft_strlen(buf_env[i]);
            pip->exp_env[i] = ft_calloc(sizeof(char), (z + 1));
            ft_strlcpy(pip->exp_env[i], "declare -x ", z);
            ft_strlcpy(&(pip->exp_env[i][11]), buf_env[i], ft_strlen(buf_env[i]) + 1);
            i++;
        }
    }
    else
    {
        while (buf_env[i])
        {
            z = ft_strlen(buf_env[i]);
            pip->exp_env[i] = ft_calloc(sizeof(char), (z + 1));
            ft_strlcpy(pip->exp_env[i], buf_env[i], ft_strlen(buf_env[i]) + 1);
            i++;
        } 
    }
    pip->exp_env[i] = NULL;
    anihilation(buf_env);
    return (0);
}

static int check_if_double(char **env, char *cmp)
{
    int i;
    int trigger;

    i = 0;
    trigger = 0;
    while (env[i])
    {
        if (strncmp(env[i], cmp, ft_strlen(cmp)) == 0)
            trigger++;
        i++;
    }
    if (trigger > 1)
        return (1);
    return (0);
}

static char **ft_cpy_env(char **env)
{
    int i;
    char **buf_env;

    i = 0;
    while (env[i])
        i++;
    buf_env = malloc(sizeof(char *) * (i + 1));
    i = 0;
    while (env[i])
    {
        buf_env[i] = ft_strdup(env[i]);
        i++;
    }
    buf_env[i] = NULL;
    return (buf_env);
}

int add_exp_env(t_cmd *pip, char *str)
{
    int  i;
    char **new_one;
    char *buf;

    buf = malloc(sizeof(char) * (ft_strlen(str) + 12));
    if (!buf)
        return (1);
    ft_strlcpy(buf, "declare -x ", 12);
    ft_strlcpy(&(buf[11]), str, ft_strlen(str) + 1);
    i = 0;
    while (pip->exp_env[i])
    {
        if (strncmp(buf, pip->exp_env[i], ft_strlen(buf)) == 0)
        {
            free(buf);
            return (0);
        }
        i++;
    }
    while (pip->exp_env[i])
        i++;
    new_one = malloc(sizeof(char*) * (i + 2));
    if (new_one == NULL)
        return (free(buf), error(MALLOC, 0), 1);
    i = 0;
    while (pip->exp_env[i])
    {
        new_one[i] = ft_strdup(pip->exp_env[i]);
        if (!new_one[i])
            return (free(new_one), free(buf), 1);
        i++;
    }
    new_one[i] = buf;
    new_one[i + 1] = NULL;
    anihilation(pip->exp_env);
    pip->exp_env = new_one;
    initialize_exp_env(pip, pip->exp_env);
    return (0);
}
