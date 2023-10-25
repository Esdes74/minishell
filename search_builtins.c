/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:08:50 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/25 19:44:20 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void determine_echo_or_cd(char **str, int *option);

int parent_builtins(t_cmd *pip, char **exec_cmd)
{
    int     ret;
    int     i;
    // char    *str;

    pip->builtin = FALSE;
    i = 1;
    if (ft_strlen(exec_cmd[0]) == 4 && ft_strncmp(exec_cmd[0], "exit", 4) == 0)
        return (intermediate_exit(exec_cmd), -1);
    else if (ft_strlen(exec_cmd[0]) == 2 && ft_strncmp(exec_cmd[0], "cd", 2) == 0)
    {
        pip->parent_builtin = TRUE;
        if (exec_cmd[1] == NULL || (exec_cmd[1][0] == '~' && !exec_cmd[2] && (!exec_cmd[1][1] || (exec_cmd[1][1] == '/' && !exec_cmd[1][2]))))
        {
            i = 0;
            if (exec_cmd[1])
                free (exec_cmd[1]);
            if (chdir("/") == -1)
                return (perror("Error "), status = 1, 1);
            while (pip->save_path[i])
            {
                if (chdir(pip->save_path[i]) == -1)
                    return (perror("Error "), status = 1, 1);
                i++;
            }
            return (1);
        }
        return (cd(exec_cmd, pip), 1);
    }
    else if (ft_strlen(exec_cmd[0]) == 6 && ft_strncmp(exec_cmd[0], "export", 6) == 0)
    {
        pip->parent_builtin = TRUE;
        if (!exec_cmd[i])
            return (pip->parent_builtin = FALSE, 0);
        while (exec_cmd[i] != NULL)
        {
            ret = export(pip, exec_cmd[i]);
            if (ret == 1)
                return (-1);
            i++;
        }
        return (2);
    }
    else if (ft_strlen(exec_cmd[0]) == 5 && ft_strncmp(exec_cmd[0], "unset", 5) == 0)
    {
        pip->parent_builtin = TRUE;
        while (exec_cmd[i] != NULL)
        {
            if (unset(pip, exec_cmd[i]) == 1)
                return (-1);
            i++;
        }
        return (3);
    }
    return (0);
}

int search_builtins(char **spt, t_cmd *pip)
{
    int     option;

    option = 0;
    if (spt == NULL || !spt[0])
        return (1);
    if (ft_strlen(spt[0]) == 3 && ft_strncmp(spt[0], "pwd", 3) == 0)
    {
        if (spt[1] && spt[1][0] == '-')
            return (ft_printf_fd(2, "Error : invalid option\n"), status = 2, 1);
        return (free(pwd()), 1);
    }
    else if (ft_strlen(spt[0]) == 4 && ft_strncmp(spt[0], "exit", 4) == 0)
        return (intermediate_exit(spt), 1);
    else if (ft_strlen(spt[0]) == 3 && ft_strncmp(spt[0], "env", 3) == 0)
        return (env(pip->env), 1);
    else if ((ft_strlen(spt[0]) == 4 && ft_strncmp(spt[0], "echo", 4) == 0) \
    || (ft_strlen(spt[0]) == 2 && ft_strncmp(spt[0], "cd", 2) == 0))
    {
        determine_echo_or_cd(spt, &option);
        if (option == 0 || option == 1)
            return (echo(spt, option), status = 0, 1);
        else if (option == 2)
            return (1);
    }
    else if ((ft_strlen(spt[0]) == 6 && ft_strncmp(spt[0], "export", 6) == 0) && spt[1] == NULL)
        return (print_export(pip->exp_env), 1);
    else if ((ft_strlen(spt[0]) == 6 && ft_strncmp(spt[0], "export", 6) == 0) \
    || (ft_strlen(spt[0]) == 5 && ft_strncmp(spt[0], "unset", 5) == 0))
        return (1);
    return (0);
}

static void determine_echo_or_cd(char **str, int *option)
{
    if (ft_strlen(str[0]) == 4 && ft_strncmp(str[0], "echo", 4) == 0)
    {
        if (str[1] == NULL)
            return ;
        else if (ft_strncmp(str[1], "-n", 2) == 0)
        {
            *option = 1;
            if (str[2] == NULL)
                return ;
        }
    }
    else if (ft_strlen(str[0]) == 2 && ft_strncmp(str[0], "cd", 2) == 0)
    {
        *option = 1;
        if (str[1] == NULL)
            return ;
        *option = 2;
        if (str[2] != NULL) // TODO: Bulle a résoudre
        {
            error(TOO_MANY_ARG, str[0]);
            return ;
        }
    }
    else
        *option = 3;
    return ;
}
