/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 11:08:59 by eslamber          #+#    #+#             */
/*   Updated: 2023/07/03 11:09:02 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define ERR STDERR_FILENO
static void    error_bis(t_error err, char *cmd);

void    error(t_error err, char *cmd)
{
    if (err == ADD_LIST)
        ft_printf_fd(ERR, "Error : Problem when adding pid to list.\n");
    else if (err == MALLOC)
        ft_printf_fd(ERR, "Error : Problem with malloc function.\n");
    else if (err == CALLOC)
        ft_printf_fd(ERR, "Error : Problem with calloc function.\n");
    else if (err == TAILING)
        ft_printf_fd(ERR, "Error : Problem tailing PID tailing in list.\n");
    else if (err == CMD)
        ft_printf_fd(ERR, "Error : Command not found %s\n", cmd);
    else if (err == JOIN)
        ft_printf_fd(ERR, "Error : Problem with strjoin function\n");
    else if (err == PIPE)
        ft_printf_fd(ERR, "Error : Problem with pipe function\n");
    else if (err == FORK)
        ft_printf_fd(ERR, "Error : Problem with the fork function\n");
    else if (err == SPLIT)
        ft_printf_fd(ERR, "Error : Problem with split function\n");
    error_bis(err, cmd);
    if (cmd == NULL)
        exit(1);
}

static void    error_bis(t_error err, char *cmd)
{
    if (err == EXEC)
        ft_printf_fd(ERR, "Error : Problem with the execve function\n");
    else if (err == TOO_MANY_ARG)
        ft_printf_fd(ERR, "Error : %s: too many arguments\n", cmd);
    else if (err == CLOSE)
        ft_printf_fd(ERR, "Error : Problem with clausing function\n");
    else if (err == TOKEN)
        ft_printf_fd(ERR, "Error : Syntax error near unexpected token `\\n`\n");
    else if (err == OPEN)
        ft_printf_fd(ERR, "Error : Problem with open function\n");
    else if (err == DUP)
        ft_printf_fd(ERR, "Error : Problem with dup2 function\n");
    else if (err == STRDUP)
        ft_printf_fd(ERR, "Error : Problem with ft_strdup function\n");
    else if (err == WRITE)
        ft_printf_fd(ERR, "Error : Problem with write function\n");
    else if (err == SYNTAX)
        ft_printf_fd(ERR, "Error : Syntax error near unexpected token %s\n", cmd);
    else if (err == NUM_ARG)
        ft_printf_fd(ERR, "Error : numeric argument required\n");
}
