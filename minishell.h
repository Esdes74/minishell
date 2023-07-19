/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:35:11 by eslamber          #+#    #+#             */
/*   Updated: 2023/07/05 11:52:30 by eslamber         ###   ########.fr       */
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

# define KILL_ALL SIGUSR2
# define SCAN SIGUSR1
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
    PIPE,
    FORK,
    SPLIT,
    EXEC,
}   t_error;

typedef struct t_cmd
{
    char    *infile;
    char    *outfile;
    int     nb_pipe;
    int     pipe;
    int     nb_proc;
    int     heredoc;
} t_cmd;

void	signals(void);

int     add_list(pid_t data, t_list *list);

void    error(t_error err, char *cmd);

void	quit(void);

char	*cmd_build(char *str, char **env);

void    historic_fct(char *bufff, char *test);

// Builtins
char    *pwd(void);

void    env(char **envir);

void    exitt(void);

void    echo(char *arg, int option);

void    cd(char *path);
void    anihilation(char **str);

int     close_all_pipes(t_cmd *pi);

char	*ft_strjoin_pip(char *dest, char *src);

int     cmd_center(char *str, t_cmd *fd, char **env);
#endif
