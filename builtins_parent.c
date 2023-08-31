/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 10:31:07 by dbaule            #+#    #+#             */
/*   Updated: 2023/08/31 23:51:53 by dbaule           ###   ########.fr       */
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

int export(char ***env, char *name_value)
{
    char    *var_name;
    int     i;
    int     z;
    char    **new_env;
    char    **buf;

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
    buf = *env;
    while (buf[i])
    {
        if (strncmp(buf[i], var_name, (ft_strlen(var_name))) == 0)
        {
            char *buff;
            
            buff = ft_strdup(name_value);
            free(buf[i]); // je comprend pas pourquoi ça ne marchais pas *(env)[i]
            buf[i] = buff;
            break ;
        }
        i++;
    }
    if (buf[i] == NULL)
    {
        new_env = malloc(sizeof(char *) * (i + 2));
        if (!new_env)
            return (1);
        i = 0;
        while (buf[i])
        {
            new_env[i] = buf[i];
            i++;
        }
        new_env[i] = name_value;
        new_env[i + 1] = NULL;
        *(env) = new_env;
    }
    return 0;
}

int unset(char ***env, char *name_value)
{
    int     i;
    char    **buf;
    char    **new_env;
    int     trigger;

    i = 0;
    trigger = 0;
    buf = *env;
    while (buf[i])
        i++;
    new_env = ft_calloc(sizeof(char*), i);
    if (!new_env)
        return(error(MALLOC, '\0'), 1);
    i = 0;
    while (buf[i])
    {
        if (strncmp(buf[i], name_value, (ft_strlen(name_value))) == 0)
        {
            trigger = 1;
            i++;
        }
        if (!buf[i])
            break;
        new_env[i] = ft_strdup(buf[i]);
        i++;
    }
    // new_env[i + 1] = '\0';
    if (trigger == 1)
        *(env) = new_env;
    else
        anihilation(new_env);
    return (0);
}
