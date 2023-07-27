/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_center.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:17:58 by marvin            #+#    #+#             */
/*   Updated: 2023/07/04 15:17:58 by marvin           ###   ########.fr       */
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

int cmd_center_simple(char *str, char **env) //j'ai enlever la condtion si env != NULL 
{
    int     check;
    char    **spt;

    spt = ft_split(str, ' ');
    if (spt == NULL)
        return (error(SPLIT, NULL), 1);
    else if (spt[0] == 0)
        return (0);
    check = search_builtins(spt, env);
    if (check == 1) 
        return (anihilation(spt), 1);
    if (check == 0)
        return (0);
    if (execute_child(env, spt) == 1)
        return (1); 
    return (0);
}

static int search_builtins(char **spt, char **envi)
{
    int     option;

    option = 0;
    // while (str[i] == ' ') // actuellement  le probleme c'est si on met pas le echo en premier par exemple, il ne fonctionnera pas, mais avec un split avec les pipes ça dne devrais pas poser de soucis
    //     i++;
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

	cmd = cmd_build(str[0], environ);
	if (cmd == NULL)
		return (anihilation(str), 1);
	// if (close_all_pipes(pip) == 1)
	// 	return (free(cmd), anihilation(splitted), 2);
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
        if (str[2] != NULL)
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