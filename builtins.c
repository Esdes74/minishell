/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 11:08:50 by eslamber          #+#    #+#             */
/*   Updated: 2023/08/29 11:08:54 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *pwd(void)
{
    char *cmd;

    cmd = getcwd(NULL, 0);
    if (cmd == NULL)
        return (exit(EXIT_FAILURE), cmd);
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

    flag = 0;
    i = 1 + option;
    while (arg[i])
    {
        j = 1;
        while (arg[i][j] && arg[i][j] != '"' && arg[i][j] != '\'')
            j++;
        if ((arg[i][j] == '"' || arg[i][j] == '\'') && arg[i][j + 1] != '\0')
        {
            if (arg[i][0] == '"' || arg[i][0] == '\'')
                j = 1;
            else
                j = 0;
            while (arg[i][j])
            {
                if (arg[i][j] != '"' && arg[i][j] != '\'')
                {
                    flag = 0;
                    ft_printf("%c", arg[i][j]);
                }
                else if ((arg[i][j] == '"' || arg[i][j] == '\'') && flag == 0)
                {
                    flag = 1;
                    ft_printf(" ");
                }
                j++;
            }
        }
        else
        {
            flag = 0;
            arg[i][j] = '\0';
            if (arg[i][0] == '"' || arg[i][0] == '\'')
                ft_printf(&arg[i][1]);
            else
                ft_printf(arg[i]);
            if ((arg[i][0] == '"' || arg[i][0] == '\'') && flag == 0)
            {
                flag = 1;
                j = 0;
                while (arg[i][j] != '\0' && arg[i][j] != '\n')
                    j++;
                if (arg[i][j] != '\n' && arg[i + 1] != NULL)
                    ft_printf("\n");
            }
            else if ((arg[i][0] == '"' || arg[i][0] == '\'') && flag == 1)
                flag = 0;
            if (flag == 0)
                ft_printf(" ");
        }
        i++;
    }
    if (option == 0)
        ft_printf("\n");
}

void    quit(void)
{
	printf("exit\n");
    silent_quit();
}