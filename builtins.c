/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaule <dbaule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 11:08:50 by eslamber          #+#    #+#             */
/*   Updated: 2023/10/19 23:38:09 by dbaule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int redirect_info(char **arg);

char    *pwd(void)
{
    char *cmd;

    cmd = getcwd(NULL, 0);
    if (cmd == NULL)
        return (NULL);
    ft_printf("%s\n", cmd);
    return (cmd);
}

void    env(char **envir)
{
    int i;

    i = 0;
    while (envir[i])
        ft_printf("%s\n", envir[i++]);
}

void    echo(char **arg, int option)
{
    int i;
    int j;
    int flag;
    int type_flag;

    i = 1;
    flag = 0;
    j = 0;
    status = 0;
    // if (redirect_info(arg) == 0)
    // {
    //     dup2(1, STDOUT_FILENO);
    // }
    if (option == 1)
        i = position_echo_n(arg, &j, &flag, &option);
    type_flag = 0;
    while (arg[i])
    {
        if (flag != 2)
            j = 1;
        if (flag == 2)
            flag = 0;
        while (arg[i][j] && arg[i][j] != '"' && arg[i][j] != '\'')
            j++;
        if ((arg[i][j] == '"' || arg[i][j] == '\'') && arg[i][j + 1] != '\0')
        {
            if (arg[i][0] == '"')
            {
                type_flag = 1;
                j = 1;
            }
            else if (arg[i][0] == '\'')
            {
                type_flag = 2;
                j = 1;
            }
            else
                j = 0;
            if (type_flag == 1)
                while (arg[i][j])
                {
                    if (arg[i][j] != '"')
                    {
                        flag = 0;
                        ft_printf_fd(1, "%c", arg[i][j]);
                    }
                    else if (arg[i][j] == '"' && flag == 0)
                        flag = 1;
                    j++;
                }
            else if (type_flag == 2)
                while (arg[i][j])
                {
                    if (arg[i][j] != '\'')
                    {
                        flag = 0;
                        ft_printf_fd(1, "%c", arg[i][j]);
                    }
                    else if (arg[i][j] == '\'' && flag == 0)
                        flag = 1;
                    j++;
                }
            else
                while (arg[i][j])
                {
                    if (arg[i][j] != '"' && arg[i][j] != '\'')
                    {
                        flag = 0;
                        ft_printf_fd(1, "%c", arg[i][j]);
                    }
                    else if (arg[i][j] == '\'' && flag == 0)
                        flag = 1;
                    j++;
                }
        }
        else
        {
            flag = 0;
            arg[i][j] = '\0';
            if (arg[i][0] == '"' || arg[i][0] == '\'')
                ft_printf_fd(1, &arg[i][1]);
            else
                ft_printf_fd(1, arg[i]);
            if ((arg[i][0] == '"' || arg[i][0] == '\'') && flag == 0)
            {
                flag = 1;
                j = 0;
                while (arg[i][j] != '\0' && arg[i][j] != '\n')
                    j++;
                if (arg[i][j] != '\n' && arg[i + 1] != NULL)
                    ft_printf_fd(1, "\n");
            }
            else if ((arg[i][0] == '"' || arg[i][0] == '\'') && flag == 1)
                flag = 0;
            if (arg[i + 1] && flag == 0)
                ft_printf_fd(1, " ");
        }
        i++;
    }
    if (option == 0)
        ft_printf_fd(1, "\n");
}

void    quit(void)
{
	printf("exit\n");
    silent_quit();
}

// static int redirect_info(char **arg)
// {
//     int i;

//     i = 0;
//     while(arg[i])
//     {
//         if (arg[i][0] == '>')
//             return (1);
//         i++;
//     }
//     return (0);
// }
