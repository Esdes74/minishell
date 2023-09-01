/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 10:31:07 by dbaule            #+#    #+#             */
/*   Updated: 2023/09/01 15:03:55 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    exitt(void)
{
    quit();
}

void    cd(char *path)
{
    if (path != NULL && chdir(path) != 0)
        ft_printf("-bash: cd: %s: No such file or directory\n");
}

int export(t_cmd *pip, char *name_value)
{
    char    *var_name;
    int     i;
    int     z;
    char    **new_env;

    i = 0;
    if (name_value[i] == '=') // ca doit faire une erreur
        return (1);
    while (name_value[i] && name_value[i] != '=')
        i++;
    if (name_value[i] == '\0') // ca fais pas une erreur mais il fais rien
    {
        return (0);
    }
    var_name = malloc(sizeof(char) * (i + 1));
    if (!var_name)
    {
        return (1);
    }
    z = i;
    i = 0;
    while (i < z)
    {
        var_name[i] = name_value[i];
        i++;
    }
    var_name[i] = '\0';
    i = 0;
    while (pip->env[i])
    {
        if (strncmp(pip->env[i], var_name, (ft_strlen(var_name))) == 0)
        {
            char *buff;
            
            buff = ft_strdup(name_value);
            free(pip->env[i]);
            pip->env[i] = buff;
            break ;
        }
        i++;
    }
    if (pip->env[i] == NULL)
    {
        new_env = malloc(sizeof(char *) * (i + 2));
        if (!new_env)
            return (free_all(pip), 1);
        i = 0;
        while (pip->env[i])
        {
            new_env[i] = pip->env[i];
            i++;
        }
        new_env[i] = name_value;
        new_env[i + 1] = NULL;
        pip->env = new_env;
    }
    return 0;
}

int unset(t_cmd *pip, char *name_value)
{
    int     i;
    char    **new_env;
    int     trigger;

    i = 0;
    trigger = 0;
    while (pip->env[i])
    {
        if (strncmp(pip->env[i], name_value, (ft_strlen(name_value))) == 0)
            trigger = 1;
        i++;
    }
    if (trigger == 0)
        return (0);
    new_env = ft_calloc(sizeof(char*), i);
    if (!new_env)
        return(error(MALLOC, '\0'), 1);
    i = 0;
    while (pip->env[i])
    {
        if (strncmp(pip->env[i], name_value, (ft_strlen(name_value))) == 0)
        {
            free(pip->env[i]);
            i++;
        }
        if (!pip->env[i])
            break;
        new_env[i] = pip->env[i];
        i++;
    }
    pip->env = new_env;
    return (0);
}
