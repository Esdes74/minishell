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
    TOO_MANY_ARG,
    CLOSE,
}   t_error;

typedef struct t_cmd
{
    char    *infile;
    char    *outfile;
    int     nb_pipe;
    int     **pipe;
    int     nb_proc;
    int     heredoc;
} t_cmd;

// Générals
void	signals(void);

int     add_list(pid_t data, t_list *list);

void    error(t_error err, char *cmd);

void	quit(void);

void    historic_fct(char *bufff, char *test);

char    *verif_read(char *rd_line, char **env);

// Builtins
char    *pwd(void);

void    env(char **envir);

void    exitt(void);

void    echo(char **arg, int option);

void    cd(char *path);

// Fork and pipes
void    anihilation(char **str);

int     close_all_pipes(t_cmd *pi);

char	*ft_strjoin_pip(char *dest, char *src);

// Commands
char	*cmd_build(char *str, char **env);

int     cmd_center_simple(char **str, char **env);

int     parsing_check(char *rd_line);

void    second_parsing_check(char *rd_line, int *flag, t_list *spt);

int     *counting_arg(t_cell *cell, int count, t_list *spt);

char    **string_for_cmd_center(int *tab, t_cell *cell, int i, t_list *spt);

int     execution_center(t_list *spt, char **env);
#endif
