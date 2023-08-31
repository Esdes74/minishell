/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 10:31:07 by dbaule            #+#    #+#             */
/*   Updated: 2023/08/31 14:48:20 by dbaule           ###   ########.fr       */
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
    while (name_value[i] != '=')
        i++;
    if (name_value == NULL)
    {
        return (2);
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
        if (strncmp(buf[i], var_name, (ft_strlen(var_name) - 1)) == 0)
        {
            // free(*(env)[i]);
            *(env)[i] = ft_strdup(name_value);
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
        ft_printf("new_env %s\n", name_value);
        new_env[i] = name_value;
        new_env[i + 1] = NULL;
        *(env) = new_env;
    }
    return 0;
}


// unset ce sera même délire mais je malloc moins que le vrai, je copie et j'unset la premiere strncmp que je croise je la prend pas en compte


// int unset(char ***env, char *name_value)
// {
//     char    *var_name;
//     int     i;
//     int     z;
//     char    **new_env;
//     char    **buf;

//     i = 0;
//     while (name_value[i] != '=')
//         i++;
//     if (name_value == NULL)
//     {
//         return (2);
//     }
//     var_name = malloc(sizeof(char) * (i + 1));
//     if (!var_name)
//     {
//         return (1);
//     }
//     z = i;
//     i = 0;
//     while (i < z)
//     {
//         var_name[i] = name_value[i];
//         i++;
//     }
//     var_name[i] = '\0';
//     i = 0;
//     buf = *env;
//     while (buf[i])
//     {
//         if (strncmp(buf[i], var_name, (ft_strlen(var_name) - 1)) == 0)
//         {
//             *(env)[i] = "\0";
//             break ;
//         }
//         i++;
//     }
//     if (buf[i] == NULL)
//     {
//         new_env = malloc(sizeof(char *) * (i + 2));
//         if (!new_env)
//             return (1);
//         i = 0;
//         while (buf[i])
//         {
//             new_env[i] = buf[i];
//             i++;
//         }
//         new_env[i] = name_value;
//         printf("new_env %s\n", new_env[i]);
//         new_env[i + 1] = NULL;
//         *(env) = new_env;
//     }
//     return 0;
// }