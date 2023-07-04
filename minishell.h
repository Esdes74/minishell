/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:35:11 by eslamber          #+#    #+#             */
/*   Updated: 2023/07/03 14:10:33 by eslamber         ###   ########.fr       */
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
# include <sys/wait.h>

# include "libft/libft.h"

# define KILL_ALL -3
# define SCAN -2
# define QUIT -1
# define CON_C SIGINT
# define DEBUG 1

extern t_list  *list;

typedef enum    e_error{
    ADD_LIST,
    MALLOC,
    TAILING,
    CMD,
    JOIN,
}   t_error;

void	signals(void);

int     add_list(pid_t data, t_list *list);

void    error(t_error err, char *cmd);

void	quit(void);

char	*cmd_build(char *str, char **env);

void    historic_fct(char *bufff, char *test);
#endif
