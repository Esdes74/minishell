/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_center.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:17:58 by eslamber          #+#    #+#             */
/*   Updated: 2023/07/04 15:17:58 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
-   il faut prendre en compte entré et sortie en amont
-   uniquement la string que on lui envoie doit s'executer
-   le nombre de pipe et le nombre de processeur sera établis en amont
-   les dup et la fermeture des pipes en amont
-      en amont pour le heredoc
*/

#include "minishell.h"
static int  search_builtins(char **str, char **envi);
static int	execute_child(char **environ, char **str);
static char *determine_echo_or_cd(char **str, int *option);
static char *return_buf(char **spt);

int cmd_center_simple(char **str, char **env) //j'ai enlever la condtion si env != NULL 
{
    int     check;

    check = search_builtins(str, env);
    if (check == 1) 
        return (anihilation(str), 1);
    if (check == -1)
        return (0);
    if (execute_child(env, str) == 1)
        return (1);
    return (0);
}

static int search_builtins(char **spt, char **envi)
{
    int     option;

    option = 0;
    if (ft_strlen(spt[0]) == 3 && ft_strncmp(spt[0], "pwd", 3) == 0)
        return (pwd(), -1);
    else if (ft_strlen(spt[0]) == 4 && ft_strncmp(spt[0], "exit", 4) == 0)
        return (exitt(), -1);
    else if (ft_strlen(spt[0]) == 3 && ft_strncmp(spt[0], "env", 3) == 0)
        return (env(envi), -1);
    else if ((ft_strlen(spt[0]) == 4 && ft_strncmp(spt[0], "echo", 4) == 0) \
    || (ft_strlen(spt[0]) == 2 && ft_strncmp(spt[0], "cd", 2) == 0))
    {
        if (determine_echo_or_cd(spt, &option) == NULL)
            return (error(MALLOC, NULL), 1);
        if (option == 0 || option == 1)
            return (echo(spt, option), -1);
        else if (option == 2)
            return (cd(spt[1]), -1);
    }
    return (0);
}

static int	execute_child(char **environ, char **str)
{
	char	*cmd;
    char    *false_cmd;

	cmd = cmd_build(str[0], environ);
	if (cmd == NULL)
    {
        false_cmd = ft_calloc(1, 1);
        if (!false_cmd)
            return (free(str), 1);
        if (write(STDOUT_FILENO, false_cmd, 1) == -1)
            return (free (false_cmd), free(str), 1);
		return (free(str), free(false_cmd), exit(EXIT_FAILURE), 1);
    }
    annihilation(list, free, DEBUG);
	execve(cmd, str, environ);
	error(EXEC, "0");
	return (free(cmd), anihilation(str), 2);
}

static char     *determine_echo_or_cd(char **str, int *option)
{
    if (ft_strlen(str[0]) == 4 && ft_strncmp(str[0], "echo", 4) == 0)
    {
        if (str[1] == NULL)
            return (return_buf(str));
        else if (ft_strlen(str[1]) == 2 && ft_strncmp(str[1], "-n", 2) == 0)
        {
            *option = 1;
            if (str[2] == NULL)
                return (return_buf(str));
        }
    }
    else if (ft_strlen(str[0]) == 2 && ft_strncmp(str[0], "cd", 2) == 0)
    {
        *option = 1;
        if (str[1] == NULL)
            return (return_buf(str));
        *option = 2;
        if (str[2] != NULL) // TODO: Bulle a résoudre
            return (error(TOO_MANY_ARG, str[0]), return_buf(str));
    }
    else
        *option = 3;
    return (NULL);
}

static char *return_buf(char **spt)
{
    char *buf;

    buf = ft_calloc(1, 1);
    if (buf == NULL)
        return (anihilation(spt), error(MALLOC, NULL), NULL);
    return (buf);
}
