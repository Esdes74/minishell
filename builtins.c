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
    printf("%s\n", cmd);
    return (cmd);
}

void    exitt(void)
{
    quit();
}

void    env(char **envir)
{
    int i;

    i = 0;
    while (envir[i])
        ft_printf(envir[i++]);
}

void    echo(char **arg, int option)
{
    int i;

    i = 1 + option;
    while (arg[i])
    {
        ft_printf(arg[i]);
        ft_printf(" ");
        i++;
    }
    if (option == 0)
        ft_printf("\n");
}

void    cd(char *path)
{
    if (path != NULL && chdir(path) != 0)
        ft_printf("-bash: cd: %s: No such file or directory\n");
}
