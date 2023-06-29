/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:35:11 by eslamber          #+#    #+#             */
/*   Updated: 2023/06/27 16:53:02 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "libft/libft.h"

# define KILL_ALL -1
# define SCAN -2
# define CON_C SIGINT
# define DEBUG 1

t_list  list;

typedef enum    e_error{
    ADD_LIST,
    MALLOC,
    TAILING,
}   t_error;

void	signals(void);

int     add_list(pid_t data, t_list *list);

void    error(t_error err);

#endif
