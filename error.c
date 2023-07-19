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

void    error(t_error err, char *cmd)
{
    if (err == ADD_LIST)
        ft_printf_fd(ERR, "Error : Problem when adding pid to list.\n");
    if (err == MALLOC)
        ft_printf_fd(ERR, "Error : Problem with malloc function.\n");
    if (err == TAILING)
        ft_printf_fd(ERR, "Error : Problem tailing PID tailing in list.\n");
    if (err == CMD)
        ft_printf_fd(ERR, "Error : command not found %s\n", cmd);
    if (err == JOIN)
        ft_printf_fd(ERR, "Error : Problem with strjoin function\n");
    if (err == PIPE)
        ft_printf_fd(ERR, "Error : Problem with pipe function\n");
    if (err == FORK)
        ft_printf_fd(ERR, "Error : Problem with the fork function\n");
    if (err == SPLIT)
        ft_printf_fd(ERR, "Error : Problem with split function\n");
    if (err == EXEC)
        ft_printf_fd(ERR, "Error: problem with the execve function\n");
    if (cmd == NULL)
        exit(1);
}
