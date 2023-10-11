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
    CALLOC,
    TAILING,
    CMD,
    JOIN,
    PIPE,
    FORK,
    SPLIT,
    EXEC,
    TOO_MANY_ARG,
    CLOSE,
    TOKEN,
    OPEN,
    DUP,
    STRDUP,
    WRITE,
    SYNTAX,
}   t_error;

typedef struct  t_cmd
{
    int     nb_pipe;
    int     **pipe;
    int     nb_proc;
    int     parent_builtin;
    int     builtin; //utilisé
    int     heredoc; // utilisé
    int     ind_hd; // utilisé
    int     status; // utilisé
    int     *here_pipe; // utilisé
    char    **hd_history; // utilisé
    char    **env;
    char    **exp_env;
    t_bool  in; // utilisé
    t_bool  out; // utilisé
}   t_cmd;

// Générals
void	signals(void);

int     add_list(pid_t data, t_list *list);

void    error(t_error err, char *cmd);

void	quit(void);

void    silent_quit(void);

int     historic_fct(char *bufff, char *test, t_cmd *pip);

char    *verif_read(char *b, char *rd_line, t_cmd *pip);

int     cpy_env(char **env, t_cmd *pip);

// Builtins
int     search_parent_builtins(t_cmd *pip, t_list *spt);

int     search_builtins(char **spt, t_cmd *pip);

char    *pwd(void);

void    env(char **envir);

int    exitt(t_cmd *pip, t_list *tmp);

void    echo(char **arg, int option);

void    cd(char *path, t_cmd *pip, t_list *spt);

int     export(t_cmd *pip, char *name_value);

void    print_export(char **exp_env);

int     initialize_exp_env(t_cmd *pip, char **env);

int     add_exp_env(t_cmd *pip, char *str);

int     unset(t_cmd *pip, char *name_value);

// Free functions

void    anihilation(char **str);

void	free_all(t_cmd *pip);


// Fork and pipes
int     close_all_pipes(t_cmd *pi);

char	*ft_strjoin_pip(char *dest, char *src);

// Commands
int     check_variables(t_list *spt, t_cmd *pip);

char	*cmd_build(char *str, char **env);

int     cmd_center_simple(char **str, t_cmd *pip);

void    parsing(const char *rd_line, int *flag, t_list *spt);

int     *counting_arg(int count, t_list *spt);

char    **string_for_cmd_center(int *tab, int i, t_list *spt);

char    **check_redirection(char **arg, t_cmd *pip);

char    **prep_hd(t_cmd *pip, t_list *spt);

int     execution_center(t_list *spt, t_cmd *pip);
#endif
